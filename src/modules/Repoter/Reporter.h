/*
 * Author: Benoît Barbier
 */

#pragma once

#include <ArduinoJson.h>
#include <HTTPClient.h>

#include "../../library/index.h"
#include "../Server/index.h"

class Reporter {
   public:
    Reporter();

    // Reporter Setter
    void setNewReporting(String target_ip, int target_port, int target_sp);

    // Reporting
    void handlePeriodicReporting(const SensorDataProvider& sensorData, FireDetection& fireDetector,
                                 TemperatureRegulator& regulator, ActuatorManager& actuatorManager,
                                 WiFiModule& wifiModule);

    String getTargetIP() const noexcept { return target_ip; }

    int getTargetPort() const noexcept { return target_port; }

    int getTargetSP() const noexcept { return target_sp; }

   private:
    String reporter_route = "/esp";
    String target_ip = "";
    int target_port = 0;
    int target_sp = 0;

    // Report timer
    unsigned long tick = 0;
    Clock& clock = Clock::sharedInstance();

    bool isReporting();
    void send(HTTPClient& http, String& jsonString);
    String buildReportUrl();
    String preparerJsonPayload(const SensorDataProvider& sensorData, FireDetection& fireDetector,
                               TemperatureRegulator& regulator, ActuatorManager& actuatorManager,
                               WiFiModule& wifiModule);
    void configureHTTTP(HTTPClient& http);

    Logger logger;
};
