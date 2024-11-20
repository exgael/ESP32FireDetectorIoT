#pragma once

#include "SensorManager.h"

#include <EasyLife.h>
#include <map>

/**
 * @brief Simply hold pool status data
 */
class ESPPoolStatus {
   public:
    ESPPoolStatus(const ESPPoolStatus &other);
    ESPPoolStatus(
        String id,
        double lat,
        double lon,
        bool hotspot,
        bool occuped,
        double temperature);

    String id;
    Location location;
    bool hotspot;
    bool occuped;
    double temperature;
};

class AmIHotspot {
   public:
    AmIHotspot(SensorManager &sensorManager, double lat, double lon);

    /**
     * @Brief return whether it is an hotspot in a radius (km).
     */
    bool isHotSpot(double radius) const;

    /**
     * @brief Add an esppoolstatus.
     */
    void add(ESPPoolStatus &&other);

   private:
    Location location;
    SensorManager &sensorManager;

    /**
     * @brief This map an Identification to it values
     */
    std::map<String, ESPPoolStatus> fleet;
};