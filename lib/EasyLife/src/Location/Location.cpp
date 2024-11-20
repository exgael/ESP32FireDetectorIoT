#include "Location.h"

#include <cmath>
#define EARTH_RADIUS_METERS 6378000

Location::Location(const Location &other)
    : latitude(other.latitude), longitude(other.longitude)
{
}
Location::Location(double latitude, double longitude)
    : latitude(latitude * M_PI / 180.0), longitude(longitude * M_PI / 180.0)
{
}
double Location::distance(const Location &coor) const
{
    // Harversin
    double s1 = sin((coor.latitude - latitude) / 2);
    double s2 = sin((coor.longitude - longitude) / 2);
    return 2 * EARTH_RADIUS_METERS *
        asin(sqrt(s1 * s1 + cos(latitude) * cos(coor.latitude) * s2 * s2));
}