/*
 * Author: Benoît Barbier
 */


#include "Reporter.h"

#include "../ESPConfig.h"

Reporter::Reporter() : logger("Reporter") {}

void Reporter::setNewReporting(String target_ip, int target_port, int target_sp) {
    this->target_ip = target_ip;
    this->target_port = target_port;
    this->target_sp = target_sp;
}

/**
 * @brief Check if Repotering is enabled and sufficient time has passed before
 * sending
 */
void Reporter::handlePeriodicReporting(const SensorDataProvider& sensorData, FireDetection& fireDetector,
                                       TemperatureRegulator& regulator, ActuatorManager& actuatorManager,
                                       WiFiModule& wifiModule) {
    if (isReporting() && clock.hasTimePassed(tick, target_sp * 1000)) {
        String jsonString = preparerJsonPayload(sensorData, fireDetector, regulator, actuatorManager, wifiModule);
        logger.debug(jsonString.c_str());
        HTTPClient http;
        configureHTTTP(http);
        send(http, jsonString);
        http.end();
    }
}

void Reporter::send(HTTPClient& http, String& jsonString) {
    // Send the request
    int httpResponseCode = http.POST(jsonString);

    // Verify response code
    if (httpResponseCode >= 200 && httpResponseCode < 300) {
        logger.info("Data sent successfully, response code: %d", httpResponseCode);
    } else {
        String errorMessage = http.getString();  // Retrieve response for debugging
        logger.error("Error sending data, code: %d, message: %s", httpResponseCode, errorMessage.c_str());
    }
}

/**
 * @brief Whether report is occuring
 */
bool Reporter::isReporting() { return target_sp > 0 && !target_ip.isEmpty(); }

String Reporter::buildReportUrl() { return "http://" + target_ip + ":" + String(target_port) + reporter_route; }

String Reporter::preparerJsonPayload(const SensorDataProvider& sensorData, FireDetection& fireDetector,
                                     TemperatureRegulator& regulator, ActuatorManager& actuatorManager,
                                     WiFiModule& wifiModule) {
    JsonDocument json;

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
    location["room"] = ESPConfig::sharedInstance().ROOM;

    JsonObject gps = location["gps"].to<JsonObject>();
    gps["lat"] = ESPConfig::sharedInstance().LATITUDE;
    gps["lon"] = ESPConfig::sharedInstance().LONGITUDE;

    location["address"] = ESPConfig::sharedInstance().ADDRESS;

    // TemperatureRegulator settings object
    JsonObject regul = json["regul"].to<JsonObject>();
    regul["lt"] = regulator.getLowThreshold();
    regul["ht"] = regulator.getHighThreshold();

    // Info object
    JsonObject info = json["info"].to<JsonObject>();
    info["ident"] = ESPConfig::sharedInstance().IDENT;
    info["user"] = ESPConfig::sharedInstance().USER;
    info["loc"] = ESPConfig::sharedInstance().LOC;

    // Network status object
    JsonObject net = json["net"].to<JsonObject>();
    net["uptime"] = millis() / 1000;
    net["ssid"] = wifiModule.getSSID();
    net["mac"] = wifiModule.getMAC();
    net["ip"] = wifiModule.getIP();

    // Report host configuration object
    JsonObject Repoterhost = json["reporthost"].to<JsonObject>();
    Repoterhost["target_ip"] = target_ip;
    Repoterhost["target_port"] = target_port;
    Repoterhost["sp"] = target_sp;

    String jsonString;
    serializeJson(json, jsonString);
    return jsonString;
}

void Reporter::configureHTTTP(HTTPClient& http) {
    http.begin(buildReportUrl());
    http.addHeader("Content-Type", "application/json");
}
