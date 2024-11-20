/*
 * Author: Benoît Barbier
 */

#include "ESPConfig.h"
#include "Reporter.h"

Reporter::Reporter() : logger("Reporter")
{
}

void Reporter::setNewReporting(String target_ip, int target_port, int target_sp)
{
    this->target_ip = target_ip;
    this->target_port = target_port;
    this->target_sp = target_sp;
}

/**
 * @brief Check if Repotering is enabled and sufficient time has passed before
 * sending
 */
void Reporter::handlePeriodicReporting(
    const SensorManager &sensorData,
    FireDetector &fireDetector,
    TemperatureRegulator &regulator,
    AmIHotspot& hotspot,
    ActuatorManager &actuatorManager,
    WiFiModule &wifiModule)
{
    if (isReporting() && clock.hasTimePassed(tick, target_sp * 1000)) {
        
        String jsonString = PayloadMaker::getCompleteStateString(
            sensorData, 
            fireDetector, 
            regulator, 
            hotspot,
            actuatorManager, 
            wifiModule, 
            *this
        );

        logger.debug(jsonString.c_str());
        HTTPClient http;
        configureHTTTP(http);
        send(http, jsonString);
        http.end();
    }
}

void Reporter::send(HTTPClient &http, String &jsonString)
{
    // Send the request
    int httpResponseCode = http.POST(jsonString);

    // Verify response code
    if (httpResponseCode >= 200 && httpResponseCode < 300) {
        logger.info(
            "Data sent successfully, response code: %d", httpResponseCode);
    } else {
        String errorMessage =
            http.getString(); // Retrieve response for debugging
        logger.error(
            "Error sending data, code: %d, message: %s",
            httpResponseCode,
            errorMessage.c_str());
    }
}

/**
 * @brief Whether report is occuring
 */
bool Reporter::isReporting()
{
    return target_sp > 0 && !target_ip.isEmpty();
}

String Reporter::buildReportUrl()
{
    return "http://" + target_ip + ":" + String(target_port) + reporter_route;
}

void Reporter::configureHTTTP(HTTPClient &http)
{
    http.begin(buildReportUrl());
    http.addHeader("Content-Type", "application/json");
}
