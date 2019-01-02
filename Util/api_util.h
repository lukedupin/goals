#ifndef API_UTIL_H
#define API_UTIL_H

#include <QString>
#include <QJsonValue>
#include <QJSEngine>
#include <QJSValue>

//Calls a js value, doing the conversion for us
bool callQJSValue( QJSValue& value, QJSEngine* engine );
bool callQJSValue( QJSValue& value, QJSEngine* engine, QJsonValue val0 );
bool callQJSValue( QJSValue& value, QJSEngine* engine, QJsonValue val0, QJsonValue val1 );

//Provides a path to convert from QJsonValue to QJValue
QJSValue toQJSValue( QJSEngine* engine, const QJsonValue& val );

//Convert a QVariant into a QJsonValue
QJsonValue variantToQJsonValue( QVariant value );

#endif
