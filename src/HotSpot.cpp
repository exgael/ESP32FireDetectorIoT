#include "Hotspot.h"

Hotspot::Hotspot(SensorManager &sensorManager, double lat, double lon)
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

String Hotspot::toString() const
{
    String newline = "\n";

    if (this->lastNearHotspot == nullptr) {
        return newline;
    }
    return lastNearHotspot->toString() + newline;;
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

int Hotspot::add(ESPPoolStatus &&other)
{
    if (other.location.distance(this->location) <= 10) {
        // Self is hotspot
        if (this->lastNearHotspot == nullptr && other.temperature > this->sensorManager.getTemperature()) {
            this->lastNearHotspot = &other;
            return 2;
        }

        // Self is not hotspot
        if (this->lastNearHotspot->temperature < other.temperature) {
            lastNearHotspot = &other;
            return 2;
        }

        // Self by default
        if (this->lastNearHotspot != nullptr) {
            if (this->lastNearHotspot->temperature < this->sensorManager.getTemperature()) {
                lastNearHotspot = nullptr;
                return 2;
            }
        }
    }

    return 1;
}

bool Hotspot::isHotspot() const
{
    return this->lastNearHotspot == nullptr;
}
