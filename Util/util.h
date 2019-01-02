#ifndef UTIL_H
#define UTIL_H

#include <QtCore>

#include <QString>

QString cheap_uuid();

//Define my is inside function
template<typename T>
inline bool isInside( T check, T mid, T range )
{
    return ( mid - range <= check && check <= mid + range );
}

#endif
