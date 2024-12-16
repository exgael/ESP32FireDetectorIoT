#include "Server/Controller.h"

/////////////////////////////
//          ROOT           //
/////////////////////////////

Handler rootControllerHandler(
    ActuatorManager &actuatorManager,
    SensorManager &sensorManager,
    Hotspot &hotspot,
    WiFiModule &wifiModule) noexcept
{
    return [&](Request &req, Response &res) {
        std::map<String, std::function<String()>> placeholderMap = {
            { "TEMPERATURE",
                [&]() { return String(sensorManager.getTemperature()); } },
            { "LIGHT",
                [&]() { return String(sensorManager.getLuminosity()); } },
            { "UPTIME",
                [&]() { return String(millis() / 1000); } }, // Uptime in seconds
            { "SSID", 
                [&]() { return wifiModule.getSSID(); } },
            { "MAC", 
                [&]() { return wifiModule.getMAC(); } },
            { "IP", 
                [&]() { return wifiModule.getIP(); } },
            { "IS_HOTSPOT", 
                [&]() { return String(hotspot.isHotspot());} }
        };

        /**
         * @brief Helper used for the templatting.
         *
         * @arg var: String passed corresponds to a queried value.
         * @return String: The queried value is returned as a String.
         */
        std::function<String(const String &)> processTemplate =
            [&](const String &var) {
                auto it = placeholderMap.find(var);
                while (it != placeholderMap.end()) {
                    return it->second();
                }
                return String();
            };

        // Send index.html file
        res.sendFile("/index.html", processTemplate);
    };
}

/////////////////////////////
//          FETCH          //
/////////////////////////////

Handler getValuesControllerHandler(
    SensorManager &sensorManager,
    Hotspot &hotspot) noexcept
{
    return [&](Request &req, Response &res) {
        JsonDocument json;
        json.clear();

        // Valid parameters and their handlers
        struct ParamHandler {
            const char *name;
            std::function<void(JsonDocument &)> handle;
        };

        ParamHandler paramHandlers[] = {
            { "temperature",
              [&](JsonDocument &json) {
                  json["temperature"] = sensorManager.getTemperature();
              } },
            { "light",
              [&](JsonDocument &json) {
                  json["light"] = sensorManager.getLuminosity();
              } },
            { "is_hotspot",
              [&](JsonDocument &json) {
                  json["is_hotspot"] = hotspot.isHotspot();
              } },
            { "uptime",
              [&](JsonDocument &json) { json["uptime"] = millis() / 1000; } },
        };

        /**
         * @brief Helper lambda to handle each valid parameter
         *
         * It receives the parameter name, and uses `paramHandlers` to find the
         * correct handler.
         */
        auto handleParam = [&](const String &paramName) {
            for (const auto &handler : paramHandlers) {
                if (paramName == handler.name) {
                    handler.handle(json);
                    break;
                }
            }
        };

        if (req.getQueryParamCount() == 0) {
            for (const auto &handler : paramHandlers) {
                handler.handle(json);
            }
        } else {
            for (const auto &paramName : req.getQueryParamNames()) {
                handleParam(paramName);
            }
        }

        res.sendJson(200, json);
    };
}

/////////////////////////////
//           SET           //
/////////////////////////////

Handler setValuesControllerHandler(
    ActuatorManager &actuatorManager,
    SensorManager &sensorManager)
{
    return [&](Request &req, Response &res) {
        // Process parameter by name
        for (const auto &paramName : req.getQueryParamNames()) {
            String paramValue = req.getQueryParamValue(paramName);

            // Process "cool" parameter
            if (paramName == "onboardLed") {
                if (paramValue == "on") {
                    actuatorManager.requestOnboardLedOn();
                } else if (paramValue == "off") {
                    actuatorManager.requestOnboardLedOff();
                }
            }
            // Process "heat" parameter
            else if (paramName == "ledStrip") {
                if (paramValue == "on") {
                    actuatorManager.requestLedStripOn();
                } else if (paramValue == "off") {
                    actuatorManager.requestLedStripOff();
                }
            }
            // Process threshold parameters
            else if (paramName == "ledStripColor") {
                if (paramValue == "red") {
                    actuatorManager.requestLedStripRed();
                } else if (paramValue == "green") {
                    actuatorManager.requestLedStripGreen();
                } else if (paramValue == "orange") {
                    actuatorManager.requestLedStripOrange();
                }
            }
        }

        // Send confirmation response
        res.send(200);
    };
}