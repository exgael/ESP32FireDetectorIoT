/*
 * Author: Benoît Barbier
 */

#pragma once

#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>

#include "../../../library/index.h"
#include "../../ActuatorManager/index.h"
#include "../../FireDetector/index.h"
#include "../../SensorManager/index.h"
#include "../../TemperatureRegulator/index.h"
#include "../../WiFiModule/index.h"
#include "../express/index.h"

class Reporter;

/**
 * @brief This class containes all the server controllers.
 */
class Controllers {
   public:
    Controllers(const SensorDataProvider& sensorData, FireDetection& fireDetector, TemperatureRegulator& regulator,
                ActuatorManager& actuatorManager, Reporter& reporter, WiFiModule& wifiModule);

    void rootController(Request& req, Response& res) noexcept;
    void getValuesController(Request& req, Response& res) const noexcept;
    void setValuesController(Request& req, Response& res);
    void setReportController(Request& req, Response& res) noexcept;
    void sendReportConfigStatus(Request& req, Response& res) const noexcept;
    void sendPeriodicReport();

   private:
    const SensorDataProvider& sensorData;
    FireDetection& fireDetector;
    TemperatureRegulator& regulator;
    ActuatorManager& actuatorManager;
    Reporter& reporter;
    WiFiModule& wifiModule;

    Logger logger;

    void sendParamBadRequest(JsonDocument& json, const String& paramName, Response& res) const noexcept;
    void sendParamValueBadRequest(JsonDocument& json, const String& paramName, const String& paramValue,
                                  Response& res) const noexcept;
    void sendNoValidParamResponse(JsonDocument& json, Response& res, const String& validParams) const noexcept;
};
