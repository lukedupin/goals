#!/usr/bin/python3

import json, time, re, sys, os
from api import api_data

def tTot( btype ):
    btype = re.sub(r"^str", "QString", btype)
    btype = re.sub(r"^float", "double", btype)
    btype = re.sub(r"^list", "JArray", btype)
    btype = re.sub(r"^long", "qint64", btype)
    btype = re.sub(r"^bool", "bool", btype)
    btype = re.sub(r"<str>", "<JString>", btype)
    btype = re.sub(r"<int>", "<JInt>", btype)
    btype = re.sub(r"<bool>", "<JBool>", btype)
    btype = re.sub(r"<float>", "<JDouble>", btype)
    btype = re.sub(r"<long>", "<JDateTime>", btype)
    return btype

def tTotQml( btype ):
    btype = re.sub(r"^list<.*>", "QVariantList", btype)
    btype = re.sub(r"^str", "QString", btype)
    btype = re.sub(r"^int", "int", btype)
    btype = re.sub(r"^bool", "bool", btype)
    btype = re.sub(r"^long", "qint64", btype)
    btype = re.sub(r"^float", "double", btype)
    return btype

def read_value( btype, name, padding = "" ):
    #Base type, just output the name
    if re.search(r'^str', btype) is not None or \
       re.search(r'^int', btype) is not None or \
       re.search(r'^float', btype) is not None or \
       re.search(r'^bool', btype) is not None or \
       re.search(r'^long', btype) is not None:
        return name

    # Whatever else it is, it needs to handle exportValue();
    return '%s.exportValue()' % (name)

def read_value_qml( btype, name, padding = "" ):
    #Base type, just output the name
    if re.search(r'^str', btype) is not None or \
       re.search(r'^int', btype) is not None or \
       re.search(r'^bool', btype) is not None or \
       re.search(r'^float', btype) is not None or \
       re.search(r'^long', btype) is not None:
        return name

    # Whatever else it is, it needs to handle exportValue();
    return 'variantToQJsonValue( %s )' % (name)


def handle( js, output_dir ):
    #js = json.loads( " ".join(open( schema ).exportlines()) )
    dir = output_dir + "/"

    out = open(dir+"Json/json_struct.h", "w")
    out.write('''// Generated file, please edit api.py in Scripts
#ifndef JSON_STRUCT_H
#define JSON_STRUCT_H

#include <Json/json_variable.h>

#define J( type, name ) type name = type( #name, this )

''')

    # Output the api
    for struct in js['structs']:
        out.write("\n")
        out.write("class %s : public JsonBase\n" % struct['name'] )
        out.write("{\n")
        out.write("    J_BASE( %s )\n" % struct['name'])
        out.write("\n")
        for field in struct['fields']:
            ary = field.split(' ')
            out.write("    J( %s, %s );\n" % (ary[0], ary[-1]))
        out.write("};\n")

    out.write("\n")
    out.write("void apiStructRegister();\n")
    out.write("\n")
    out.write("#endif\n")
    out.close()

    # Build out the register calls
    out = open(dir+"Json/json_struct.cpp", "w")
    out.write('#include <Json/json_struct.h>\n')
    out.write("\n")
    out.write("void apiStructRegister()\n")
    out.write("{\n")
    for struct in js['structs']:
        out.write('    qRegisterMetaType<%s>( "%s" );\n' % (struct['name'], struct['name']))
    out.write("}\n\n")
    for struct in js['structs']:
        out.write("%s::~%s() {}\n" % (struct['name'], struct['name']))
    out.close()

    # Output api.h
    out = open(dir+"Net/api.h", "w")
    out.write("#ifndef API_H\n")
    out.write("#define API_H\n")
    out.write("\n")
    out.write('#include <Json/json_struct.h>\n')
    out.write('#include <Net/network_http.h>\n')
    out.write('#include <QJSValue>\n')
    for section in js['sections']:
        out.write("\n")
        out.write("class Api%s\n" % section['name'].capitalize() )
        #out.write("class Api%s : public QObject\n" % section['name'].capitalize() )
        out.write("{\n")
        #out.write("    Q_OBJECT\n")
        #out.write("\n")
        out.write("    private:\n")
        out.write("    NetworkHttp* _http = nullptr;\n")
        out.write("\n")
        out.write("    public:\n")
        out.write("    Api%s( NetworkHttp* http );\n" % section['name'].capitalize() )

        for ep in section['endpoints']:
            out.write("\n")

            #Short form template
            if len(ep['args']) > 0:
                args = [tTot(x) for x in ep['args']]
                out.write("    void %s( %s,\n" % (ep['name'], ', '.join(args)))
            else:
                out.write("    void %s(\n" % (ep['name']))
            out.write("             std::function<void (%s&)> success = nullptr,\n" % ep['resp'])
            out.write("             std::function<void (%s&, bool)> failed = nullptr );\n" % ep['err'])

            #Long form template (Timeout is included as its own callback
            if len(ep['args']) > 0:
                args = [tTot(x) for x in ep['args']]
                out.write("    void %s( %s,\n" % (ep['name'], ', '.join(args)))
            else:
                out.write("    void %s(\n" % (ep['name']))
            out.write("             std::function<void (%s&)> success,\n" % ep['resp'])
            out.write("             std::function<void (%s&)> failed,\n" % ep['err'])
            out.write("             std::function<void ()> timeout );\n")
        out.write("};\n")

        out.write("\n")
        out.write("class Api%sQml : public QObject\n" % section['name'].capitalize() )
        out.write("{\n")
        out.write("    Q_OBJECT\n")
        out.write("\n")
        out.write("    private:\n")
        out.write("    NetworkHttp* _http = nullptr;\n")
        out.write("    QJSEngine* _engine = nullptr;\n")
        out.write("\n")
        out.write("    public:\n")
        out.write("\n")
        out.write("    void loadEngine( NetworkHttp* http, QJSEngine* engine );\n")

        for ep in section['endpoints']:
            out.write("\n")
            if len(ep['args']) > 0:
                args = [tTotQml(x) for x in ep['args']]
                out.write("    Q_INVOKABLE void %s( %s,\n" % (ep['name'], ', '.join(args)))
            else:
                out.write("    Q_INVOKABLE void %s(\n" % (ep['name']))
            out.write("                             QJSValue success = QJSValue(), QJSValue failed = QJSValue(), QJSValue timeout = QJSValue() );\n")
        out.write("};\n")


    # Create the header for the main api object
    out.write("\n")
    out.write("class ApiInterface : public QObject\n")
    out.write("{\n")
    out.write("    Q_OBJECT\n")
    out.write("\n")
    out.write("    private:\n")
    out.write("    NetworkHttp* _http = nullptr;\n")
    out.write("    bool _networkOnline = false;\n")
    out.write("\n")
    out.write("    public:\n")
    out.write("\n")
    out.write("    static const int MAJOR = 1;\n")
    out.write("    static const int MINOR = 0;\n")
    out.write("\n")
    out.write("    ApiInterface( NetworkHttp* http, Mailbox* mailbox );\n")
    out.write("\n")
    out.write("    void loadEngine( QJSEngine* engine );\n")
    out.write("\n")
    out.write("    Q_INVOKABLE bool isOnline();\n")
    out.write("\n")
    for section in js['sections']:
        out.write("    Api%s %s;\n" % (section['name'].capitalize(), section['name'].capitalize()))
    for section in js['sections']:
        out.write("\n")
        out.write("    Q_PROPERTY( Api%sQml* %s READ _%s )\n" % (section['name'].capitalize(), section['name'].capitalize(), section['name']))
        out.write("    Api%sQml _%sQml;\n" % (section['name'].capitalize(), section['name']))
        out.write("    Api%sQml* _%s();\n" % (section['name'].capitalize(), section['name']))
    out.write("\n")
    out.write("    private slots:\n")
    out.write("\n")
    out.write("    void onNetworkStatusChanged( bool online );\n")


    #Wrap up and give the user the api struct register call
    out.write("};\n")
    out.write("\n")
    out.write("#endif\n")
    out.close()

    # Build out each individual endpoint
    out = open(dir+"Net/api.cpp", "w")
    out.write('#include <functional>\n')
    out.write('#include <Net/api.h>\n')
    out.write('#include <Json/json_struct.h>\n')
    out.write('#include <Util/api_util.h>\n')
    out.write('#include <QQmlEngine>\n')
    for section in js['sections']:
        out.write("\n")
        out.write("Api%s::Api%s( NetworkHttp* network ) : _http( network ) {}\n" % (section['name'].capitalize(), section['name'].capitalize()) )
        for ep in section['endpoints']:
            for combined in (True, False):
                out.write("\n")

                if len(ep['args']) > 0:
                    args = [tTot(x) for x in ep['args']]
                    out.write("void Api%s::%s( %s,\n" % (section['name'].capitalize(), ep['name'], ', '.join(args)))
                else:
                    out.write("void Api%s::%s(\n" % (section['name'].capitalize(), ep['name']))

                if combined:
                    out.write("                std::function<void (%s&)> success,\n" % ep['resp'])
                    out.write("                std::function<void (%s&, bool)> failed )\n" % ep['err'])
                else:
                    out.write("                std::function<void (%s&)> success,\n" % ep['resp'])
                    out.write("                std::function<void (%s&)> failed,\n" % ep['err'])
                    out.write("                std::function<void ()> timeout )\n")
                out.write("{\n")
                out.write("    auto&& data = QJsonObject();\n")
                for arg in ep['args']:
                    ary = arg.split(' ')
                    out.write('    data.insert("%s", %s);\n' % (ary[-1], read_value(ary[0], ary[-1])))
                out.write('    _http->post( "/%s/%s/", data,\n' % (section['name'], ep['name']))
                out.write('                 [=](QJsonObject& js) {\n')
                out.write('                     %s resp;\n' % (ep['resp']))
                out.write('                     if ( !resp.importJson( js ) ) {\n')
                out.write('                         qDebug("Failed to map json /%s/%s/");\n' % (section['name'], ep['name']))
                out.write('                         %s err;\n' % ep['err'])
                out.write('                         auto err_map = err.importJson( js );\n')
                out.write('                         if ( failed != nullptr )\n')
                if combined:
                    out.write('                             failed( err, false );\n')
                else:
                    out.write('                             failed( err );\n')
                out.write('                         else if ( err_map )\n')
                out.write('                             qDebug("Error on /%s/%s/: %%s", err.reason.value().toLatin1().data());\n' % (section['name'], ep['name']))
                out.write('                         else\n')
                out.write('                             qDebug("Error without callback on /%s/%s/");\n' % (section['name'], ep['name']))
                out.write('                         return;\n')
                out.write('                     }\n')
                out.write('                     success( resp );\n')
                out.write('                 },\n')
                out.write('                 [=](QJsonObject& js ) {\n')
                out.write('                     %s err;\n' % ep['err'])
                out.write('                     auto err_map = err.importJson( js );\n')
                out.write('                     if ( failed != nullptr )\n')
                if combined:
                    out.write('                         failed( err, false );\n')
                else:
                    out.write('                         failed( err );\n')
                out.write('                     else if ( err_map )\n')
                out.write('                         qDebug("Error on /%s/%s/: %%s", err.reason.value().toLatin1().data());\n' % (section['name'], ep['name']))
                out.write('                     else\n')
                out.write('                         qDebug("Error without callback on /%s/%s/");\n' % (section['name'], ep['name']))
                out.write('                 },\n')
                out.write('                 [=]() {\n')
                if combined:
                    out.write('                     if ( failed != nullptr )\n')
                    out.write('                     {\n')
                    out.write('                         JErr err;\n')
                    out.write('                         err.successful = false;\n')
                    out.write('                         err.reason = "Timeout occurred";\n')
                    out.write('                         failed( err, true );\n')
                    out.write('                     }\n')
                else:
                    out.write('                     if ( timeout != nullptr )\n')
                    out.write('                         timeout();\n')
                out.write('                     else\n')
                out.write('                         qDebug("Timeout without callback on /%s/%s/");\n' % (section['name'], ep['name']))
                out.write('                 } );\n')
                out.write("}\n")

        out.write("\n")
        out.write("void Api%sQml::loadEngine( NetworkHttp* http, QJSEngine* engine )\n" % section['name'].capitalize())
        out.write("{\n")
        out.write("    _http = http;\n")
        out.write("    _engine = engine;\n")
        out.write("}\n")

        for ep in section['endpoints']:
            out.write("\n")
            if len(ep['args']) > 0:
                args = [tTotQml(x) for x in ep['args']]
                out.write("void Api%sQml::%s( %s,\n" % (section['name'].capitalize(), ep['name'], ', '.join(args)))
            else:
                out.write("void Api%sQml::%s(\n" % (section['name'].capitalize(), ep['name']))
            out.write("                QJSValue success,\n")
            out.write("                QJSValue failed,\n")
            out.write("                QJSValue timeout )\n")
            out.write("{\n")
            out.write("    auto&& data = QJsonObject();\n")
            for arg in ep['args']:
                ary = arg.split(' ')
                out.write('    data.insert("%s", %s);\n' % (ary[-1], read_value_qml(ary[0], ary[-1])))
            out.write('    _http->post( "/%s/%s/", data,\n' % (section['name'], ep['name']))
            out.write('                 [=](QJsonObject& js) mutable {\n')
            out.write('                     %s resp;\n' % (ep['resp']))
            out.write('                     if ( !resp.importJson( js ) ) {\n')
            out.write('                         qDebug("Failed to map json /%s/%s/");\n' % (section['name'], ep['name']))
            out.write('                         %s err;\n' % ep['err'])
            out.write('                         auto err_map = err.importJson( js );\n')
            out.write('                         if ( failed.isCallable() )\n')
            out.write('                             callQJSValue( failed, _engine, QJsonValue( js ) );\n')
            out.write('                         else if ( err_map )\n')
            out.write('                             qDebug("Error on /%s/%s/: %%s", err.reason.value().toLatin1().data());\n' % (section['name'], ep['name']))
            out.write('                         else\n')
            out.write('                             qDebug("Error without callback on /%s/%s/");\n' % (section['name'], ep['name']))
            out.write('                         return;\n')
            out.write('                     }\n')
            out.write('                     if ( success.isCallable() )\n')
            out.write('                         callQJSValue( success, _engine, QJsonValue( js ) );\n')
            out.write('                 },\n')
            out.write('                 [=](QJsonObject& js ) mutable {\n')
            out.write('                     %s err;\n' % ep['err'])
            out.write('                     auto err_map = err.importJson( js );\n')
            out.write('                     if ( failed.isCallable() )\n')
            out.write('                         callQJSValue( failed, _engine, QJsonValue( js ), QJsonValue( false ) );\n')
            out.write('                     else if ( err_map )\n')
            out.write('                         qDebug("Error on /%s/%s/: %%s", err.reason.value().toLatin1().data());\n' % (section['name'], ep['name']))
            out.write('                     else\n')
            out.write('                         qDebug("Error without callback on /%s/%s/");\n' % (section['name'], ep['name']))
            out.write('                 },\n')
            out.write('                 [=]() mutable {\n')
            out.write('                     if ( timeout.isCallable() )\n')
            out.write('                         callQJSValue( timeout, _engine );\n')
            out.write('                     else if ( failed.isCallable() )\n')
            out.write('                     {\n')
            out.write('                         %s err;\n' % ep['err'])
            out.write('                         err.successful = false;\n')
            out.write('                         err.reason = "Timeout occurred";\n')
            out.write('                         callQJSValue( failed, _engine, err.exportValue(), QJsonValue( true ) );\n')
            out.write('                     }\n')
            out.write('                     else\n')
            out.write('                         qDebug("Timeout without callback on /%s/%s/");\n' % (section['name'], ep['name']))
            out.write('                 } );\n')
            out.write("}\n")

    # Build out my top level api call object
    out.write("\n")
    out.write("ApiInterface::ApiInterface( NetworkHttp* http, Mailbox* mailbox ) : \n")
    out.write('    _http( http ),\n')
    out.write('%s\n' % (',\n'.join(["    %s( http )" % x['name'].capitalize() for x in js['sections']])))
    out.write("{\n")
    out.write("    QObject::connect( mailbox, &Mailbox::networkStatusChanged, this, &ApiInterface::onNetworkStatusChanged );\n")
    out.write("}\n")

    out.write("\n")
    out.write("void ApiInterface::loadEngine( QJSEngine* engine )\n")
    out.write("{\n")
    for section in js['sections']:
        out.write('    qmlRegisterType<Api%sQml>( "Radius", 1, 0, "Api%sQml");\n' % (section['name'].capitalize(), section['name'].capitalize()) )
    for section in js['sections']:
        out.write('    _%sQml.loadEngine( _http, engine );\n' % section['name'] )
    out.write("}\n")

    out.write("\n")
    out.write("bool ApiInterface::isOnline()\n")
    out.write("{\n")
    out.write("    return _networkOnline;\n")
    out.write("}\n")

    for section in js['sections']:
        out.write("\n")
        out.write("Api%sQml* ApiInterface::_%s()\n" % (section['name'].capitalize(), section['name']))
        out.write("{\n")
        out.write('    return &_%sQml;\n' % section['name'] )
        out.write("}\n")

    out.write("\n")
    out.write("void ApiInterface::onNetworkStatusChanged( bool online )\n")
    out.write("{\n")
    out.write("    _networkOnline = online;\n")
    out.write("}\n")

    out.close()

# Ensure we have enough args
if len(sys.argv) < 2:
    print("Usage ./%s output_dir" % sys.argv[0])
    exit(0)

handle( api_data(), sys.argv[1] )
#handle( sys.argv[1], sys.argv[2] )

