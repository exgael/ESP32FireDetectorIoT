#include "Server/Controller.h"

/////////////////////////////
//          ROOT           //
/////////////////////////////

Handler rootControllerHandler(
    ActuatorManager &actuatorManager,
    SensorManager &sensorManager,
    TemperatureRegulator &regulator,
    FireDetector &fireDetector,
    WiFiModule &wifiModule,
    Reporter &reporter) noexcept
{
    return [&](Request &req, Response &res) {
        std::map<String, std::function<String()>> placeholderMap = {
            { "TEMPERATURE",
              [&]() { return String(sensorManager.getTemperature()); } },
            { "LIGHT",
              [&]() { return String(sensorManager.getLuminosity()); } },
            { "UPTIME",
              [&]() { return String(millis() / 1000); } }, // Uptime in seconds
            { "RUNNING", [&]() { return regulator.describeState(); } },
            { "FIRE",
              [&]() { return fireDetector.isFireDetected() ? "Yes" : "No"; } },
            { "SSID", [&]() { return wifiModule.getSSID(); } },
            { "MAC", [&]() { return wifiModule.getMAC(); } },
            { "IP", [&]() { return wifiModule.getIP(); } },
            { "COOLER",
              [&]() { return regulator.isCooling() ? "ON" : "OFF"; } },
            { "HEATER",
              [&]() { return regulator.isHeating() ? "ON" : "OFF"; } },
            { "FTT",
              [&]() { return String(fireDetector.getTempThreshold()); } },
            { "FLT", [&]() { return String(fireDetector.getLumThreshold()); } },
            { "LT", [&]() { return String(regulator.getLowThreshold()); } },
            { "HT", [&]() { return String(regulator.getHighThreshold()); } },
            { "PRT_IP", [&]() { return reporter.getTargetIP(); } },
            { "PRT_PORT", [&]() { return String(reporter.getTargetPort()); } },
            { "PRT_T", [&]() { return String(reporter.getTargetSP()); } }
        };

        // Send index.html file
        res.sendFile("/index.html", [&](const String &var) {
            auto it = placeholderMap.find(var);
            while (it != placeholderMap.end()) {
                return it->second();
            }
            return String();
        });
    };
}

/////////////////////////////
//          FETCH          //
/////////////////////////////

Handler getValuesControllerHandler(
    SensorManager &sensorManager,
    TemperatureRegulator &regulator,
    FireDetector &fireDetector) noexcept
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
            { "running",
              [&](JsonDocument &json) {
                  json["running"] = regulator.describeState();
              } },
            { "fire",
              [&](JsonDocument &json) {
                  json["fire"] = fireDetector.isFireDetected() ? "Yes" : "No";
              } },
            { "uptime",
              [&](JsonDocument &json) { json["uptime"] = millis() / 1000; } },
            { "cooler",
              [&](JsonDocument &json) {
                  json["cooler"] = regulator.isCooling() ? "ON" : "OFF";
              } },
            { "FTT",
              [&](JsonDocument &json) {
                  json["FTT"] = fireDetector.getTempThreshold();
              } },
            { "FLT",
              [&](JsonDocument &json) {
                  json["FLT"] = fireDetector.getLumThreshold();
              } },
            { "LT",
              [&](JsonDocument &json) {
                  json["LT"] = regulator.getLowThreshold();
              } },
            { "HT",
              [&](JsonDocument &json) {
                  json["HT"] = regulator.getHighThreshold();
              } },
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
    SensorManager &sensorManager,
    TemperatureRegulator &regulator,
    FireDetector &fireDetector)
{
    return [&](Request &req, Response &res) {
        // Process parameter by name
        for (const auto &paramName : req.getQueryParamNames()) {
            String paramValue = req.getQueryParamValue(paramName);

            // Process "cool" parameter
            if (paramName == "cool") {
                if (paramValue == "on") {
                    actuatorManager.requestCoolerOn();
                } else if (paramValue == "off") {
                    actuatorManager.requestCoolerOff();
                }
            }
            // Process "heat" parameter
            else if (paramName == "heat") {
                if (paramValue == "on") {
                    actuatorManager.requestHeaterOn();
                } else if (paramValue == "off") {
                    actuatorManager.requestHeaterOff();
                }
            }
            // Process threshold parameters
            else if (paramName == "lt") {
                int newLowThreshold = paramValue.toInt();
                regulator.setLowThreshold(newLowThreshold);
            } else if (paramName == "ht") {
                int newHighThreshold = paramValue.toInt();
                regulator.setHighThreshold(newHighThreshold);
            } else if (paramName == "FTT") {
                int newFireTempThreshold = paramValue.toInt();
                fireDetector.updateTemperatureThreshold(newFireTempThreshold);
            } else if (paramName == "FLT") {
                int newFireLightThreshold = paramValue.toInt();
                fireDetector.updateLuminosityThreshold(newFireLightThreshold);
            }
        }

        // Send confirmation response
        res.send(200);
    };
}

/////////////////////////////
//          REPORT         //
/////////////////////////////

Handler setReportControllerHandler(Reporter &reporter, Logger &logger) noexcept
{
    return [&](Request &req, Response &res) {
        // Retrieve and parse target parameters
        String target_ip = req.getBodyParamValue("ip");
        int target_port_int = req.getBodyParamValue("port").toInt();
        int target_sp_int = req.getBodyParamValue("sp").toInt();

        // Set the new reporting configuration
        reporter.setNewReporting(target_ip, target_port_int, target_sp_int);

        // Log the new configuration
        logger.info(
            "Target set to IP: %s, Port: %d, Sampling Period: %d",
            target_ip.c_str(),
            target_port_int,
            target_sp_int);

        // Send confirmation response
        res.send(200);
    };
}

void sendPeriodicReport(
    ActuatorManager &actuatorManager,
    SensorManager &sensorManager,
    TemperatureRegulator &regulator,
    FireDetector &fireDetector,
    WiFiModule &wifiModule,
    Reporter &reporter)
{
    reporter.handlePeriodicReporting(
        sensorManager, fireDetector, regulator, actuatorManager, wifiModule);
}