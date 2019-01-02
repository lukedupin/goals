#include "api_util.h"

#include <QJSValueList>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>

bool callQJSValue( QJSValue& caller, QJSEngine* engine )
{
    return callQJSValue( caller, engine, QJsonValue(), QJsonValue() );
}
bool callQJSValue( QJSValue& caller, QJSEngine* engine, QJsonValue val0 )
{
    return callQJSValue( caller, engine, val0, QJsonValue() );
}
bool callQJSValue( QJSValue& caller, QJSEngine* engine, QJsonValue val0, QJsonValue val1 )
{
    if ( !caller.isCallable() )
        return false;

    //Convert the data, and then call the function
    auto&& arg0 = toQJSValue( engine, val0 );
    auto&& arg1 = toQJSValue( engine, val1 );

    //Call the function
    auto&& args = QJSValueList();
    args << arg0 << arg1;
    caller.call( args );

    return true;
}

QJSValue toQJSValue(QJSEngine* engine, const QJsonValue& val)
{
    if (val.isBool())
    {
        return QJSValue(val.toBool());
    }
    else if (val.isString())
    {
        return QJSValue(val.toString());
    }
    else if (val.isDouble())
    {
        return QJSValue(val.toDouble());
    }
    else if (val.isNull())
    {
        return QJSValue(QJSValue::NullValue);
    }
    else if (val.isUndefined())
    {
        return QJSValue(QJSValue::UndefinedValue);
    }
    else if (val.isObject())
    {
        if ( engine == nullptr )
            return QJSValue(QJSValue::UndefinedValue);

        QJsonObject obj = val.toObject();
        QJSValue newobj = engine->newObject();
        for (auto itor = obj.begin(); itor != obj.end(); itor++)
        {
            QString key = itor.key();
            QJsonValue value = itor.value();
            QJSValue convertedValue = toQJSValue(engine, value);
            newobj.setProperty(key, convertedValue);
        }
        return newobj;
    }
    else if (val.isArray())
    {
        if ( engine == nullptr )
            return QJSValue(QJSValue::UndefinedValue);

        QJsonArray arr = val.toArray();
        QJSValue newobj = engine->newArray((uint)arr.size());
        for (int i = 0; i < arr.size(); i++)
        {
            QJsonValue value = arr[i];
            QJSValue convertedValue = toQJSValue(engine, value);
            newobj.setProperty((uint)i, convertedValue);
        }
        return newobj;
    }

    // ASSERT(FALSE && "This shouldn't happen");
    return QJSValue(QJSValue::UndefinedValue);
}


//Convert a QVariant into a QJsonValue
QJsonValue variantToQJsonValue( QVariant value )
{
    //If this is a list, recursively call this list
    if ( value.type() == QVariant::List )
    {
        QJsonArray ary;
        for ( auto entry : value.toList() )
            ary.push_back( variantToQJsonValue(entry) );

        return QJsonValue(ary);
    }

    //Convert the type
    switch ( value.type() )
    {
        case QVariant::Bool: return QJsonValue( value.toBool() );
        case QVariant::Int: return QJsonValue( value.toInt() );
        case QVariant::Double: return QJsonValue( value.toDouble() );
        case QVariant::String: return QJsonValue( value.toString() );
        case QVariant::DateTime: return QJsonValue( value.toDateTime().toMSecsSinceEpoch() );
        case QVariant::Map:
        case QVariant::Hash:
            return QJsonValue( value.toJsonObject() );

        default:
            qDebug("Couldn't decode variant type: %s", value.typeName() );
            return QJsonValue();
    }
}
