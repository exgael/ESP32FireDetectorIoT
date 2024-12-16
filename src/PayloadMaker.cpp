#include "PayloadMaker.h"

String PayloadMaker::getPiscineStateString(
    const SensorManager &sensorData,
    const Hotspot &hotspot)
{
    JsonDocument json;
    json.clear();

    auto &config = ESPConfig::sharedInstance();

    // Status object
    JsonObject status = json["status"].to<JsonObject>();
    status["temperature"] = sensorData.getTemperature();

    // Location object
    JsonObject location = json["location"].to<JsonObject>();
    location["room"] = config.ROOM;

    JsonObject gps = location["gps"].to<JsonObject>();
    gps["lat"] = config.LATITUDE;
    gps["lon"] = config.LONGITUDE;

    // Info object
    JsonObject info = json["info"].to<JsonObject>();
    info["ident"] = config.IDENT;
    info["user"] = config.USER;
    info["loc"] = config.LOC;

    // Piscine object
    JsonObject piscine = json["piscine"].to<JsonObject>();
    piscine["hotspot"] = hotspot.isHotspot();
    piscine["occuped"] =
        sensorData.getLuminosity() < config.OCCUPATION_THRESHOLD ? true : false;

    return getStringFromJson(json);
}

String PayloadMaker::getStringFromJson(JsonDocument &doc)
{
    String jsonString;
    serializeJson(doc, jsonString);
    return jsonString;
}