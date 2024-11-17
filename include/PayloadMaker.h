#pragma once

// https://arduinojson.org/v7/api/jsondocument/
#include <ArduinoJson.h>

#include "ESPConfig.h"
#include "FireDetector.h"
#include "SensorManager.h"
#include "TemperatureRegulator.h"
#include "WiFiModule.h"

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
            const ActuatorManager &actuatorManager,
            const WiFiModule &wifiModule,
            const Reporter &reporter
        );

    private:
        static String getStringFromJson(JsonDocument& doc);
};
