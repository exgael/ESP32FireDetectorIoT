/*
 * Author: Benoît Barbier
 * Created: 2024-11-07
 * Last Modified: 2024-11-07
 */


#pragma once

#define USE_SERIAL Serial

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

// Config
#include "ESPConfig.h"

// MODULES
#include "FireDetector/index.h"
#include "Repoter/index.h"
#include "SensorManager/index.h"
#include "Server/index.h"
#include "TemperatureRegulator/index.h"
#include "WiFiModule/index.h"
#include "library/index.h"

class ESPManager {
   public:
    ESPManager(ESPServer& server, SensorManager& sensorManager, FireDetection& fireDetector,
               TemperatureRegulator& regulator, ActuatorManager& actuatorManager, Reporter& reporter,
               WiFiModule& wifiModule);

    void init();
    void executeWorkflow();

   private:
    
    ESPServer& server;
    SensorManager& sensorManager;
    FireDetection& fireDetector;
    TemperatureRegulator& regulator;
    ActuatorManager& actuatorManager;
    Reporter& reporter;
    WiFiModule& wifiModule;

    unsigned long tick = 0;
    unsigned long interval = 5000;
    unsigned long iter = 0;

    Logger logger;
};
