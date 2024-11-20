#include "PayloadMaker.h"
#include "Reporter.h"

String PayloadMaker::getCompleteStateString(
    const SensorManager &sensorData,
    const FireDetector &fireDetector,
    const TemperatureRegulator &regulator,
    const AmIHotspot &hotspot,
    const ActuatorManager &actuatorManager,
    const WiFiModule &wifiModule,
    const Reporter &reporter)
{
    JsonDocument json;
    json.clear();

    auto &config = ESPConfig::sharedInstance();

    // Status object
    JsonObject status = json["status"].to<JsonObject>();
    status["temperature"] = sensorData.getTemperature();
    status["light"] = sensorData.getLuminosity();
    status["regul"] = regulator.describeState();
    status["fire"] = fireDetector.isFireDetected();
    status["heat"] = regulator.isHeating() == true ? "ON" : "OFF";
    status["cold"] = regulator.isCooling() == true ? "ON" : "OFF";
    status["fanspeed"] = actuatorManager.getFanSpeed();

    // Location object
    JsonObject location = json["location"].to<JsonObject>();
    location["room"] = config.ROOM;

    JsonObject gps = location["gps"].to<JsonObject>();
    gps["lat"] = config.LATITUDE;
    gps["lon"] = config.LONGITUDE;

    location["address"] = config.ADDRESS;

    // TemperatureRegulator settings object
    JsonObject regul = json["regul"].to<JsonObject>();
    regul["lt"] = regulator.getLowThreshold();
    regul["ht"] = regulator.getHighThreshold();

    // Info object
    JsonObject info = json["info"].to<JsonObject>();
    info["ident"] = config.IDENT;
    info["user"] = config.USER;
    info["loc"] = config.LOC;

    // Network status object
    JsonObject net = json["net"].to<JsonObject>();
    net["uptime"] = millis() / 1000;
    net["ssid"] = wifiModule.getSSID();
    net["mac"] = wifiModule.getMAC();
    net["ip"] = wifiModule.getIP();

    // Report host configuration object
    JsonObject repoterhost = json["reporthost"].to<JsonObject>();
    repoterhost["target_ip"] = reporter.getTargetIP();
    repoterhost["target_port"] = reporter.getTargetPort();
    repoterhost["sp"] = reporter.getTargetSP();

    // Piscine object
    JsonObject piscine = json["piscine"].to<JsonObject>();
    piscine["hotspot"] = hotspot.isHotSpot(config.HOTSPOT_DETECTION_RADIUS);
    piscine["occuped"] =
        sensorData.getLuminosity() < config.OCCUPATION_THRESHOLD ? true : false;

    return getStringFromJson(json);
}

String PayloadMaker::getPiscineStateString(
    const SensorManager &sensorData,
    const AmIHotspot &hotspot)
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
    piscine["hotspot"] = hotspot.isHotSpot(config.HOTSPOT_DETECTION_RADIUS);
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