/*
 * Author: Benoît Barbier
 */

#pragma once

#define USE_SERIAL Serial

#include "ESPConfig.h"
#include "FireDetector.h"
#include "HotSpot.h"
#include "PayloadMaker.h"
#include "Reporter.h"
#include "SensorManager.h"
#include "TemperatureRegulator.h"
#include "WiFiModule.h"

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <EasyLife.h>
#include <SPIFFS.h>

class ESPManager {
   public:
    ESPManager(
        EasyServer &server,
        EasyMQTT &mqttClient,
        SensorManager &sensorManager,
        FireDetector &fireDetector,
        TemperatureRegulator &regulator,
        AmIHotspot &hotspot,
        ActuatorManager &actuatorManager,
        Reporter &reporter,
        WiFiModule &wifiModule);

    void init();
    void executeWorkflow();

   private:
    EasyServer &server;
    EasyMQTT &mqttClient;
    SensorManager &sensorManager;
    FireDetector &fireDetector;
    TemperatureRegulator &regulator;
    AmIHotspot &hotspot;
    ActuatorManager &actuatorManager;
    Reporter &reporter;
    WiFiModule &wifiModule;

    unsigned long tick = 0;
    unsigned long interval = 5000;
    unsigned long iter = 0;
    bool executingMainPipeline = true;
    Logger logger;
};
