#ifndef PLATFORM_H
#define PLATFORM_H

#include <QObject>

namespace PlatformTypes
{
    Q_NAMESPACE
    enum PlatformType
    {
        IOS,
        ANDRIOD,
        DESKTOP,
        ERR,
    };
    Q_ENUM_NS(PlatformType)
};

class Platform : public QObject
{
    Q_OBJECT

    public:
    static const int MAJOR = 1;
    static const int MINOR = 0;

    public:
    Q_INVOKABLE int getDeviceType();
};

#endif
