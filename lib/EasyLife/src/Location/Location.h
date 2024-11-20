#pragma once

class Location {
   public:
    Location(const Location &other);

    /**
     * @brief Stores lattitude and longitude
     */
    Location(double latitude, double longitude);

    /**
     * @brief uses Haversine.
     */
    double distance(const Location &) const;

   private:
    /**
     * @brief Latitude in rad
     */
    double latitude;

    /**
     * @brief longitude in rad
     */
    double longitude;
};