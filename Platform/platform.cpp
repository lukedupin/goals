#include "platform.h"

int Platform::getDeviceType()
{
#ifdef Q_OS_ANDROID
    return PlatformTypes::ANDRIOD;
#else
#ifdef Q_OS_IOS
    return PlatformTypes::IOS;
#else
#ifdef Q_OS_LINUX
    return PlatformTypes::DESKTOP;
#else
#error "Couldn't pick a valid platform"
    return PlatformTypes::ERR;
#endif
#endif
#endif
}
