/*
 * Author: Benoît Barbier
 */

#pragma once

#define USE_SERIAL Serial

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <EasyLife.h>

#include "Reporter.h"
#include "ESPConfig.h"
#include "FireDetector.h"
#include "SensorManager.h"
#include "TemperatureRegulator.h"
#include "WiFiModule.h"

class ESPManager {
   public:
    ESPManager(
        EasyServer& server,
        SensorManager& sensorManager, 
        FireDetector& fireDetector,
        TemperatureRegulator& regulator, 
        ActuatorManager& actuatorManager, 
        Reporter& reporter,
        WiFiModule& wifiModule
    );

    void init();
    void executeWorkflow();

   private:
    EasyServer& server;
    SensorManager& sensorManager;
    FireDetector& fireDetector;
    TemperatureRegulator& regulator;
    ActuatorManager& actuatorManager;
    Reporter& reporter;
    WiFiModule& wifiModule;

    unsigned long tick = 0;
    unsigned long interval = 5000;
    unsigned long iter = 0;
    bool executingMainPipeline = true;
    Logger logger;
};
