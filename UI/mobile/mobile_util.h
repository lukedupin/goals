#ifndef MOBILE_UTIL_H
#define MOBILE_UTIL_H

#include <QString>
#include <Json/json_struct.h>

namespace UiMobileUtil
{
    Q_NAMESPACE
    enum Relationship
    {
        SELF,
        FRIEND,
        //.. Many more I'm sure but for now we just want to show ourselves versus others
    };
    Q_ENUM_NS(Relationship)
};


//Holds my entry position info
struct EntryPosition
{
    double Lat;
    double Lng;
    double Elv;
    double Spd;
    double Dist;
    double CaptureDist;
    qint64 Ts;

    //Create an object from json
    static EntryPosition create( const JDevicePoint* raw, double capture_distance, qint64 delta );
};

//Holds a single entry
struct Entry
{
    QString Name;
    QString Act;
    int ClockSkew = 0;
    UiMobileUtil::Relationship Relation;
    QString Uid;
    QVector<EntryPosition> Data;

    //The current position we are to report on
    EntryPosition Cache;

    //Sorted add, Adds an item, sorts if need be
    QPair<int, int> addSorted( EntryPosition pos );

    //Sorted add of list, sorts if need be at the end
    QPair<int, int> addListSorted( QList<EntryPosition> pos );

    //Sort my data via timestamp
    void sortData();

    //Prune away old entries that wont be used anymore
    void prune( qint64 ticks );
};

#endif
