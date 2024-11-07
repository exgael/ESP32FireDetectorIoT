/*
 * Author: Benoît Barbier
 */


#pragma once

#include <ESPAsyncWebServer.h>

#include "../../library/index.h"

#include "../ActuatorManager/index.h"
#include "../FireDetector/index.h"
#include "../SensorManager/index.h"
#include "../TemperatureRegulator/index.h"
#include "../WiFiModule/index.h"

#include "controllers/index.h"
#include "express/index.h"

class Reporter;

class ESPServer {
   public:
    ESPServer(AsyncWebServer& server, const SensorDataProvider& sensorData, FireDetection& fireDetector,
              TemperatureRegulator& regulator, ActuatorManager& actuatorManager, Reporter& reporter, WiFiModule& wifiModule);

    void init() noexcept;

    /**
     * @brief Runs tasks that handles timing themselves.
     */
    void periodicRun();

   private:
    AsyncWebServer& server;

    Router router;
    Controllers controllers;

    void setupGlobalMiddlewares() noexcept;
    void setupRoutes() noexcept;

    Logger logger;
};
