#ifndef JSON_VARIABLE_BASE_H
#define JSON_VARIABLE_BASE_H

#include <QList>
#include <QString>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QJSValue>

//Base variable that can be used to read/write values
class JsonVariableBase
{
    protected:
        //Holds a key if I'm nested
    QString _key;

    public:
    JsonVariableBase(QString key);
    virtual ~JsonVariableBase() {}

        //Provide my key
    virtual QString key();

        //Extended by the base to take a json object
    virtual bool importValue( QJsonValue val ) = 0;

        //Return a json value
    virtual QJsonValue exportValue() const = 0;

        //Extended by the base to take a json object
    virtual bool importQJSValue( QJSValue val ) = 0;

        //Return a json value
    virtual QJSValue exportQJSValue() const = 0;

        //Reset this local value
    virtual void reset() = 0;

        //Print debug info
    virtual void debugDump( int padding = 0 ) const = 0;
};

#endif
