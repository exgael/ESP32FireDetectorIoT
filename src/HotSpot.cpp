#include "HotSpot.h"

AmIHotspot::AmIHotspot(SensorManager &sensorManager, double lat, double lon)
    : sensorManager(sensorManager), location(lat, lon)
{
}

ESPPoolStatus::ESPPoolStatus(const ESPPoolStatus &other)
    : id(other.id),
      location(other.location),
      hotspot(other.hotspot),
      occuped(other.occuped),
      temperature(other.temperature)
{
}

String ESPPoolStatus::toString() const
{
    return "ID: " + this->id + " Loc: " + this->location.toString() +
        " T: " + this->temperature + "C" + " Occuped: " + this->occuped +
        " Hotspot" + this->hotspot;
}

String AmIHotspot::toString() const
{
    String str = "\n";
    for (auto it = fleet.begin(); it != fleet.end(); ++it) {
        str += it->second.toString() + "\n";
    }
    return str;
}

ESPPoolStatus::ESPPoolStatus(
    String id,
    double lat,
    double lon,
    bool hotspot,
    bool occuped,
    double temperature)
    : id(id),
      location(lat, lon),
      hotspot(hotspot),
      occuped(occuped),
      temperature(temperature)
{
}

int AmIHotspot::add(ESPPoolStatus &&other)
{
    auto it = fleet.find(other.id);
    if (it != fleet.end()) {
        // Update entry
        it->second = std::move(other);
        return 2;
    } else {
        // Create entry
        fleet.emplace(other.id, std::move(other));
        return 1;
    }
}

bool AmIHotspot::isHotSpot(double radius) const
{
    for (auto it = fleet.begin(); it != fleet.end(); ++it) {
        if (it->second.temperature > sensorManager.getTemperature() &&
            location.distance(it->second.location) <= radius) {
            return false;
        }
    }
    return true;
}
