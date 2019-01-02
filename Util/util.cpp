#include "util.h"

#include <QRegExp>
#include <QRandomGenerator64>
#include <QDateTime>
#include <QJSValueList>
#include <QJsonObject>
#include <QJsonArray>

QString cheap_uuid()
{
    //Create my auth key
    QRegExp regex("=");
    QRandomGenerator64 rand( QDateTime::currentMSecsSinceEpoch() );
    auto&& auth_raw = QString("%1").arg( rand.generate64() ).toUtf8().toBase64();
    return QString(auth_raw).replace( regex, "" );
}
