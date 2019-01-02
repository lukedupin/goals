#!/usr/bin/python3

import json, time, re, sys, os
from mailbox import mailbox_data

def tToVariant( btype ):
    if re.search(r"^list", btype) is not None:
        return "QVariantList"
    if re.search(r"^str", btype) is not None:
        return "QString"
    if re.search(r"^float", btype) is not None:
        return "double"
    if re.search(r"^long", btype) is not None:
        return "qint64"
    if re.search(r"^int", btype) is not None:
        return "int"
    if re.search(r"^bool", btype) is not None:
        return "bool"

    return "QVariant"

def tToQJSValue( btype ):
    if re.search(r"^list", btype) is not None:
        return "QVariantList"
    if re.search(r"^str", btype) is not None:
        return "QString"
    if re.search(r"^float", btype) is not None:
        return "double"
    if re.search(r"^long", btype) is not None:
        return "qint64"
    if re.search(r"^int", btype) is not None:
        return "int"
    if re.search(r"^bool", btype) is not None:
        return "bool"

    return "QJSValue"


def tTot( btype ):
    btype = re.sub(r"^str", "QString", btype)
    btype = re.sub(r"^float", "double", btype)
    btype = re.sub(r"^list", "QList", btype)
    btype = re.sub(r"^long", "qint64", btype)
    btype = re.sub(r"<str>", "<QString>", btype)
    btype = re.sub(r"<int>", "<int>", btype)
    btype = re.sub(r"<float>", "<double>", btype)
    btype = re.sub(r"<long>", "<qint64>", btype)
    btype = re.sub(r"<point>", "<QPointF>", btype)
    return btype

def cpp_to_qml( btype, name ):
    # Are we a list!!?!?!, we're going to recurse then
    match = re.search(r'^list<([^>]+)>', btype)
    if match is not None:
        entry = "entry_%s" % name
        msg  = "    QList<QVariant> ary_%s;\n" % name
        msg += "    for ( auto %s : %s )\n" % (entry, name)
        msg += "        ary_%s.push_back( %s );\n" % (name, cpp_to_qml( match[1], entry)[0])
        return ("ary_%s" % name, msg)

    #Base type, just output the name
    if re.search(r'^str', btype) is not None or \
       re.search(r'^int', btype) is not None or \
       re.search(r'^bool', btype) is not None or \
       re.search(r'^float', btype) is not None or \
       re.search(r'^long', btype) is not None:
        return "%s" % name, ""
    elif re.search(r'^point', btype) is not None:
        return "QVariant(%s)" % name, ""
    else:
        # Whatever else it is, it needs to handle exportValue();
        return 'QVariant( %s.toJson() )' % (name), ""

def qml_to_cpp( btype, name ):
    # Are we a list!!?!?!, we're going to recurse then
    match = re.search(r'^list<([^>]+)>', btype)
    if match is not None:
        entry = "entry_%s" % name
        msg  = "    QList<%s> ary_%s;\n" % (tTot(match[1]), name)
        msg += "    for ( auto %s : %s )\n" % (entry, name)
        msg += "        ary_%s.push_back( %s );\n" % (name, qml_to_cpp( match[1], entry)[0])
        return ("ary_%s" % name, msg)

    #Base type, just output the name
    if re.search(r'^str', btype) is not None or \
       re.search(r'^int', btype) is not None or \
       re.search(r'^bool', btype) is not None or \
       re.search(r'^float', btype) is not None or \
       re.search(r'^long', btype) is not None:
        return "%s" % name, ""
    elif re.search(r'^point', btype) is not None:
        return "%s.toPointF()" % name, ""
    else:
        # Whatever else it is, it needs to handle exportValue();
        return '%s( %s )' % (btype, name), ""

def capitalize( val ):
    return val[0].capitalize() + val[1:]

def emitData( emit, args, name ):
    if emit == "cpp":
        params = []
        for arg in args:
            s = arg.split(' ')
            params.append('%s %s' % (tTot(s[0]), s[1]))
        return ("emit%s" % capitalize( name ), ', '.join(params))

    elif emit == "qml":
        params = []
        for arg in args:
            s = arg.split(' ')
            params.append('%s %s' % (tToQJSValue(s[0]), s[1]))
        return ("emit%sQml" % capitalize( name ), ', '.join(params) )


def signalData( signal, args, name ):
    if signal == "cpp":
        params = []
        for arg in args:
            s = arg.split(' ')
            params.append('%s %s' % (tTot(s[0]), s[1]))
        return ( name, ', '.join(params))

    elif signal == "qml":
        params = []
        for arg in args:
            s = arg.split(' ')
            params.append('%s %s' % (tToVariant(s[0]), s[1]))
        return ( "%sQml" % name, ', '.join(params))


def callSignal( signal, emit, args, name ):
    # Setup the function name
    func_name = name
    if signal == "qml":
        func_name = "%sQml" % name

    # Same same, easy, just send stuff out
    if signal == emit:
        params = []
        for arg in args:
            s = arg.split(' ')
            params.append(s[1])
        return (func_name, ', '.join(params), None)

    # Setup the conversion logic
    signal_args = []
    signal_prep = []
    for arg in args:
        s = arg.split(' ')
        v, p = cpp_to_qml(s[0], s[1]) if signal == 'qml' else qml_to_cpp(s[0], s[1])
        signal_args.append(v)
        signal_prep.append(p)
    return (func_name, ', '.join(signal_args), '\n'.join(signal_prep))

def handle( js, output_dir ):
    #js = json.loads( " ".join(open( schema ).readlines()) )
    dir = output_dir + "/"

    out = open(dir+"Shared/mailbox.h", "w")
    out.write('''// Generated file, please edit export_mailbox.py in Scripts
#ifndef MAILBOX_H
#define MAILBOX_H

#include "Json/json_struct.h"

#include <QObject>
#include <QVariantList>
#include <QPointF>

class Mailbox : public QObject
{
    Q_OBJECT

    public:
    static const int MAJOR = 1;
    static const int MINOR = 0;

    public:
    explicit Mailbox(QObject *parent = nullptr);
    
''')

    # The invokable
    out.write("    public slots:\n")
    for section in js:
        out.write("\n")
        out.write("    //*** %s\n\n" % section['section'])
        for endpoint in section['endpoints']:
            for emit in endpoint['emit']:
                func, args = emitData( emit, endpoint['args'], endpoint['name'])
                out.write("    void %s( %s );\n" % ( func, args ))
        out.write("\n")

    # Signals
    out.write("\n")
    out.write("    signals:\n")
    for section in js:
        out.write("\n")
        out.write("    //*** %s\n\n" % section['section'])
        for endpoint in section['endpoints']:
            for signal in endpoint['signal']:
                func, args = signalData( signal, endpoint['args'], endpoint['name'] )
                out.write("    void %s( %s );\n" % ( func, args ) )

    out.write("};\n")
    out.write("\n")
    out.write("#endif\n")
    out.close()

    # Build out the register calls
    out = open(dir+"Shared/mailbox.cpp", "w")
    out.write('''
#include "mailbox.h"

Mailbox::Mailbox(QObject *parent) : QObject(parent)
{
}

''')

    # Output the data
    for section in js:
        out.write("\n")
        out.write("//*** %s\n\n" % section['section'])
        for endpoint in section['endpoints']:
            for emit in endpoint['emit']:
                emit_func, emit_args = emitData(emit, endpoint['args'], endpoint['name'] )
                out.write("void Mailbox::%s( %s  )\n" % ( emit_func, emit_args) )
                out.write("{\n")
                for signal in endpoint['signal']:
                    signal_func, signal_args, signal_prep = callSignal( signal, emit, endpoint['args'], endpoint['name'] )
                    if signal_prep is not None:
                        out.write(signal_prep)
                    out.write("    emit %s( %s );\n" % ( signal_func, signal_args ))

                out.write("}\n\n")

    out.close()

# Ensure we have enough args
if len(sys.argv) < 2:
    print("Usage ./%s output_dir" % sys.argv[0])
    exit(0)

handle( mailbox_data(), sys.argv[1] )
#handle( sys.argv[1], sys.argv[2] )

