/*
 * Author: Benoît Barbier
 */

#pragma once

#define USE_SERIAL Serial

#include "ESPConfig.h"
#include "Hotspot.h"
#include "ActuatorManager.h"
#include "PayloadMaker.h"
#include "SensorManager.h"
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
        Hotspot &hotspot,
        ActuatorManager &actuatorManager,
        WiFiModule &wifiModule);

    void init();
    void executeWorkflow();

   private:
    EasyServer &server;
    EasyMQTT &mqttClient;
    SensorManager &sensorManager;
    Hotspot &hotspot;
    ActuatorManager &actuatorManager;
    WiFiModule &wifiModule;

    unsigned long tick = 0;
    unsigned long interval = 1000;
    unsigned long iter = 0;
    bool executingMainPipeline = true;
    Logger logger;
};
