#ifndef GEO_H
#define GEO_H

#include <QtGlobal>

  //My Geo class
class Geo
{
  public:
    //Radius of earth in meters
  static constexpr double RADIUS = 6371000;

    //A lat long point
  class Point
  {
    public:
    double Lat;
    double Lng;

    Point( double lat = 0, double lng = 0 );
  };

    //Bounding box
  class BoundingBox
  {
    public:
      //Create a box based on a center and distance
    static BoundingBox box( double lat, double lng, double dist = 0 );

    public:
      //My current bounding values
    double Top;
    double Right;
    double Bottom;
    double Left;
    bool Dump = false;

      //Build out my box
    BoundingBox( double top = 0, double right = 0,
                 double bottom = 0, double left = 0 );

      //True if inside
    bool inside( double lat, double lng );

      //True if this box overlaps myself
    bool overlaps( BoundingBox& box );

      //Add a point, if its outside of the box, increase the size of the box
    BoundingBox& add( double lat, double lng );
  };

    //Return the distance
  static double distance( double lat1, double lng1, double lat2, double lng2 );

    //Return a new lat lng based on current point, distance and bearing
  static Point distanceBearing( double lat1, double lng1,
                                double dist, double bearing );

    //Return the speed based on distance and time
  static double speedDist( double m_s, qint64 ts0, qint64 ts1 );

    //Return the speed
  static double speed( double lat0, double lng0, qint64 ts0,
                       double lat1, double lng1, qint64 ts1 );
};

#endif // GEO_H
