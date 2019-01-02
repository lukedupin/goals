#include "mobile_util.h"
#include <Util/lock.h>

#include <QQmlEngine>
#include <QJsonDocument>

using namespace std;

//Return the converted object
EntryPosition EntryPosition::create( const JDevicePoint* raw, double capture_distance, qint64 delta )
{
    EntryPosition pos;
    {
        pos.Lat = raw->lat;
        pos.Lng = raw->lng;
        pos.Elv = raw->elv;
        pos.Spd = raw->spd;
        pos.Dist = raw->dist;
        pos.CaptureDist = capture_distance;
        pos.Ts = raw->ts.ms() + delta;
    }

    return pos;
}

//Sort my data entries
QPair<int, int> Entry::addListSorted( QList<EntryPosition> pos )
{
    qint64 ts = 0;
    if ( Data.length() > 0 )
        ts = Data.last().Ts;

    auto idx = Data.length();

    //Add in the data, we'll sort at the end
    bool should_sort = false;
    for ( auto& p : pos )
    {
        should_sort |= ( ts > p.Ts );
        Data.append(p);
        ts = p.Ts;
    }

    //Sort data?
    if ( should_sort )
    {
        sortData();
        return QPair<int, int>( -1, -1 );
    }

    return QPair<int, int>( idx, Data.length() - 1 );
}

//Sort my data entries
QPair<int, int> Entry::addSorted( EntryPosition pos )
{
    qint64 ts = 0;
    if ( Data.length() > 0 )
        ts = Data.last().Ts;

    //Append, and maybe sort?
    Data.append( pos );
    if ( ts > pos.Ts )
    {
        sortData();
        return QPair<int, int>( -1, -1 );
    }

    auto idx = Data.length() - 1;
    return QPair<int, int>( idx, idx );
}

//Sort my data entries
void Entry::sortData()
{
    qSort( Data.begin(), Data.end(), []( const EntryPosition& a, const EntryPosition& b) -> int {
        return a.Ts < b.Ts;
    });
}

//prune my data entries
void Entry::prune( qint64 ticks )
{
    //Loop while we have at least 2 data points, and
    //the ticks is larger than the second one, meaning 0 can't be used
    //Assuming we are sorted
    while ( Data.length() >= 2 && ticks >= Data[1].Ts )
        Data.removeFirst();
}
