#pragma once

// https://arduinojson.org/v7/api/jsondocument/
#include "ESPConfig.h"
#include "FireDetector.h"
#include "HotSpot.h"
#include "SensorManager.h"
#include "TemperatureRegulator.h"
#include "WiFiModule.h"

#include <ArduinoJson.h>

class Reporter;

class PayloadMaker {
   public:
    /**
     * @brief Resume all states of the ESP
     * @return String: A string from Json.
     */
    static String getCompleteStateString(
        const SensorManager &sensorData,
        const FireDetector &fireDetector,
        const TemperatureRegulator &regulator,
        const AmIHotspot &hotspot,
        const ActuatorManager &actuatorManager,
        const WiFiModule &wifiModule,
        const Reporter &reporter);

    /**
     * @brief Resume all states of the ESP
     * @return String: A string from Json.
     */
    static String getPiscineStateString(
        const SensorManager &sensorData,
        const AmIHotspot &hotspot);

   private:
    static String getStringFromJson(JsonDocument &doc);
};
