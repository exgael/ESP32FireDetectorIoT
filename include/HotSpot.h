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

    String toString() const;

    String id;
    Location location;
    bool hotspot;
    bool occuped;
    double temperature;
};

class Hotspot {
   public:
    Hotspot(SensorManager &sensorManager, double lat, double lon);

    /**
     * @Brief return whether it is an hotspot in a radius (km).
     */
    bool isHotspot() const;

    /**
     * @brief Add an esppoolstatus.
     *
     * @return 1 if no change, 2 if updated
     */
    int add(ESPPoolStatus &&other);

    String toString() const;

   private:
    Location location;
    SensorManager &sensorManager;

    /**
     * @brief Last status of a near hotspot
     */
    ESPPoolStatus* lastNearHotspot = nullptr;
};