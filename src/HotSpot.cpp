#include "HotSpot.h"

AmIHotspot::AmIHotspot(SensorManager& sensorManager, double lat, double lon)
    : sensorManager(sensorManager), location(lat, lon)
{
}

ESPPoolStatus::ESPPoolStatus(const ESPPoolStatus& other)
    : id(other.id), location(other.location), hotspot(other.hotspot), occuped(other.occuped), temperature(other.temperature)
{
}

ESPPoolStatus::ESPPoolStatus(String id, double lat, double lon, bool hotspot, bool occuped, double temperature) : 
    id(id), location(lat, lon), hotspot(hotspot), occuped(occuped), temperature(temperature)
{}

void AmIHotspot::add(ESPPoolStatus&& other) {
    auto it = fleet.find(other.id);
    if (it != fleet.end()) {
        // Update entry
        it->second = std::move(other);
    } else {
        // Create entry
        fleet.emplace(other.id, std::move(other));
    }
}

bool AmIHotspot::isHotSpot(double radius) const {
    for (auto it = fleet.begin(); it != fleet.end(); ++it) {
        if (it->second.temperature > sensorManager.getTemperature() &&
            location.distance(it->second.location) <= radius) {
            return false;
        }
    }
    return true;
}


