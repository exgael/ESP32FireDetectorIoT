#pragma once

// https://arduinojson.org/v7/api/jsondocument/
#include "ESPConfig.h"
#include "Hotspot.h"
#include "SensorManager.h"
#include "WiFiModule.h"

#include <ArduinoJson.h>

class PayloadMaker {
   public:
    /**
     * @brief Resume all states of the ESP
     * @return String: A string from Json.
     */
    static String getPiscineStateString(
        const SensorManager &sensorData,
        const Hotspot &hotspot);

   private:
    static String getStringFromJson(JsonDocument &doc);
};
