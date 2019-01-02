#include "geo.h"

#include <QtMath>

    //A lat long point
Geo::Point::Point( double lat, double lng ) :
  Lat( lat ),
  Lng( lng )
{}

  //Create a box based on a center and distance
Geo::BoundingBox Geo::BoundingBox::box( double lat, double lng, double dist )
{
  if ( dist <= 0 )
    return Geo::BoundingBox( lat, lng, lat, lng );

    //Get the upper right corner
  double dist2 = dist * 1.5; //A bit bigger than root 2, which is 1.414....
  Point tup = Geo::distanceBearing( lat, lng, dist2, 45.0 );

    //Calc the tup difference
  double dlat = tup.Lat - lat;
  double dlng = tup.Lng - lng;

  return BoundingBox( tup.Lat, tup.Lng, (lat - dlat), (lng - dlng));
}

  //Build out my box
Geo::BoundingBox::BoundingBox( double top, double right,
                               double bottom, double left ) :
  Top( top ),
  Right( right ),
  Bottom( bottom ),
  Left( left )
{}

  //True if inside
bool Geo::BoundingBox::inside( double lat, double lng )
{
  if ( Dump )
    qDebug("if %f >= %f and %f <= %f and %f >= %f and %f <= %f:",
           Top, lat, Bottom, lat, Right, lng, Left, lng);

    //Is this point inside?
  return (Top >= lat && Bottom <= lat && Right >= lng && Left <= lng);
}

  //True if this box overlaps myself
bool Geo::BoundingBox::overlaps( Geo::BoundingBox& box )
{
  return !(box.Top < Bottom || box.Right < Left ||
           box.Bottom > Top || box.Left > Right);
}

  //Add a point, if its outside of the box, increase the size of the box
Geo::BoundingBox& Geo::BoundingBox::add( double lat, double lng )
{
  if ( lng > Right )
    Right = lng;
  if ( lng < Left )
    Left = lng;
  if ( lat > Top )
    Top = lat;
  if ( lat < Bottom )
    Bottom = lat;

  return *this;
}

  //Return the distance
double Geo::distance( double lat1, double lng1, double lat2, double lng2)
{
    //Convert from degrees to radians
  double diff_lat = qDegreesToRadians( lat2 - lat1 );
  double diff_lng = qDegreesToRadians( lng2 - lng1 );
  lat1 = qDegreesToRadians( lat1 );
  lat2 = qDegreesToRadians( lat2 );

    //calc my distance value
  double d_sin_lat = qSin(diff_lat / 2.0);
  double d_sin_lng = qSin(diff_lng / 2.0);
  double a = (d_sin_lat * d_sin_lat) +
             (d_sin_lng * d_sin_lng) *
             (qCos(lat1) * qCos(lat2));
  double c = 2.0 * qAtan2( qSqrt(a), qSqrt(1.0-a) );

    //Convert to the earth's radius
  return RADIUS * c;
}

  //Return a new lat lng based on current point, distance and bearing
Geo::Point Geo::distanceBearing( double lat1, double lng1,
                                 double dist, double bearing )
{
    //Setup my input variables
  double br = qDegreesToRadians( bearing );
  double d = dist / RADIUS;
  lat1 = qDegreesToRadians( lat1 );
  lng1 = qDegreesToRadians( lng1 );

    //Calculate my new lat/lng
  double lat2 = qAsin( qSin(lat1) * qCos( d) +
                       qCos(lat1) * qSin( d) * qCos(br) );
  double lng2 = lng1 + qAtan2( qSin(br) * qSin( d) * qCos(lat1),
                               qCos( d) - qSin(lat1) * qSin(lat2));

  return Point( qRadiansToDegrees(lat2), qRadiansToDegrees(lng2) );
}

  //Return the speed based on distance and time
double Geo::speedDist( double m_s, qint64 ts0, qint64 ts1 )
{
  if ( ts0 == ts1 )
    return 0;

    //Get the speed
  double ts = qAbs( (double)(ts1 - ts0) / 1000000.0);
  return m_s / ts;
}

  //Return the speed
double Geo::speed( double lat0, double lng0, qint64 ts0,
                   double lat1, double lng1, qint64 ts1 )
{
  if ( ts0 == ts1 )
    return 0;

    //Get the speed
  double m_s = distance( lat0, lng0, lat1, lng1 );
  return speedDist( m_s, ts0, ts1 );
}
