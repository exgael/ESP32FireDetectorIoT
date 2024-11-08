/*
 * Author: Benoît Barbier
 */


#include "controllers.h"

#include "../../ESPConfig.h"
#include "../../Repoter/index.h"
#include <map>
#include <functional>

Controllers::Controllers(const SensorDataProvider& sensorData, FireDetection& fireDetector,
                         TemperatureRegulator& regulator, ActuatorManager& actuatorManager, Reporter& reporter,
                         WiFiModule& wifiModule)
    : sensorData(sensorData),
      fireDetector(fireDetector),
      regulator(regulator),
      actuatorManager(actuatorManager),
      reporter(reporter),
      wifiModule(wifiModule),
      logger("Controllers") {}

void Controllers::getValuesController(Request& req, Response& res) const noexcept {
    JsonDocument json;
    bool foundValidParam = false;

    // Define valid parameters and their handlers
    struct ParamHandler {
        const char* name;
        std::function<void(JsonDocument&)> handler;
    };

    ParamHandler paramHandlers[] = {
        {"temperature", [&](JsonDocument& json) { json["temperature"] = sensorData.getTemperature(); }},
        {"light", [&](JsonDocument& json) { json["light"] = sensorData.getLuminosity(); }},
        {"running", [&](JsonDocument& json) { json["running"] = regulator.describeState(); }},
        {"fire", [&](JsonDocument& json) { json["fire"] = fireDetector.isFireDetected() ? "Yes" : "No"; }},
        {"uptime", [&](JsonDocument& json) { json["uptime"] = millis() / 1000; }},
        {"cooler", [&](JsonDocument& json) { json["cooler"] = regulator.isCooling() ? "ON" : "OFF"; }},
        {"FTT", [&](JsonDocument& json) { json["FTT"] = fireDetector.getTempThreshold(); }},
        {"FLT", [&](JsonDocument& json) { json["FLT"] = fireDetector.getLumThreshold(); }},
        {"LT", [&](JsonDocument& json) { json["LT"] = regulator.getLowThreshold(); }},
        {"HT", [&](JsonDocument& json) { json["HT"] = regulator.getHighThreshold(); }},
    };

    int params = req.getParamSize();

    if (params == 0) {
        // If no parameters are provided, return all values
        for (const auto& handler : paramHandlers) {
            handler.handler(json);
        }
        foundValidParam = true;
    } else {
        for (int i = 0; i < params; i++) {
            auto param = req.getParam(i);
            if (param != nullptr) {
                String paramName = param->name();
                bool paramFound = false;

                for (const auto& handler : paramHandlers) {
                    if (paramName == handler.name) {
                        handler.handler(json);
                        foundValidParam = true;
                        paramFound = true;
                        break;
                    }
                }

                if (!paramFound) {
                    // Invalid parameter
                    sendParamBadRequest(json, paramName, res);
                    return;
                }
            }
        }
    }

    if (!foundValidParam) {
        String validParams =
            "temperature, light, running, fire, uptime, cooler, FTT, FLT, LT, "
            "HT";
        sendNoValidParamResponse(json, res, validParams);
        return;
    }

    // Send the response
    String response;
    serializeJson(json, response);
    res.sendJson(response);
}

/**
 * @brief can throw if a set is rejected
 */
void Controllers::setValuesController(Request& req, Response& res) {
    JsonDocument json;
    bool foundValidParam = false;

    int params = req.getParamSize();

    for (int i = 0; i < params; i++) {
        auto param = req.getParam(i);
        if (param != nullptr) {
            String paramName = param->name();
            String paramValue = param->value();

            if (paramName == "cool") {
                if (paramValue.equals("on")) {
                    actuatorManager.requestCoolerOn();
                    foundValidParam = true;
                } else if (paramValue.equals("off")) {
                    actuatorManager.requestCoolerOff();
                    foundValidParam = true;
                } else {
                    sendParamValueBadRequest(json, paramName, paramValue, res);
                    return;
                }
            } else if (paramName == "heat") {
                if (paramValue.equals("on")) {
                    actuatorManager.requestHeaterOn();
                    foundValidParam = true;
                } else if (paramValue.equals("off")) {
                    actuatorManager.requestHeaterOff();
                    foundValidParam = true;
                } else {
                    sendParamValueBadRequest(json, paramName, paramValue, res);
                    return;
                }
            } else if (paramName == "lt") {
                int newLowThreshold = paramValue.toInt();
                regulator.setLowThreshold(newLowThreshold);
                foundValidParam = true;
            } else if (paramName == "ht") {
                int newLowThreshold = paramValue.toInt();
                regulator.setHighThreshold(newLowThreshold);
                foundValidParam = true;
            } else if (paramName == "FTT") {
                int newFireTempThreshold = paramValue.toInt();
                fireDetector.updateTemperatureThreshold(newFireTempThreshold);
                foundValidParam = true;
            } else if (paramName == "FLT") {
                int newFireLightThreshold = paramValue.toInt();
                fireDetector.updateLuminosityThreshold(newFireLightThreshold);
                foundValidParam = true;
            } else {
                // Invalid parameter
                sendParamBadRequest(json, paramName, res);
                return;
            }
        }
    }

    if (!foundValidParam) {
        String validParams = "cool, heat, light_threshold";
        sendNoValidParamResponse(json, res, validParams);
        return;
    }

    // Send the response
    String response;
    serializeJson(json, response);
    res.sendJson(response);
}

void Controllers::sendParamBadRequest(JsonDocument& json, const String& paramName, Response& res) const noexcept {
    json.clear();
    json["error"] = "Bad Request";
    json["message"] = String("Invalid query parameter: ") + paramName;
    res.status(400);
    String response;
    serializeJson(json, response);
    res.error(response);
}

void Controllers::sendParamValueBadRequest(JsonDocument& json, const String& paramName, const String& paramValue,
                                           Response& res) const noexcept {
    json.clear();
    json["error"] = "Bad Request";
    json["message"] = String("Query parameter ") + paramName + " has invalid value " + paramValue + ".";
    res.status(400);
    String response;
    serializeJson(json, response);
    res.error(response);
}

void Controllers::sendNoValidParamResponse(JsonDocument& json, Response& res,
                                           const String& validParams) const noexcept {
    json.clear();
    json["error"] = "Bad Request";
    json["message"] = "No valid parameters provided";
    json["hint"] = String("Valid parameters: ") + validParams;
    res.status(400);
    String response;
    serializeJson(json, response);
    res.error(response);
}

void Controllers::sendPeriodicReport() {
    reporter.handlePeriodicReporting(sensorData, fireDetector, regulator, actuatorManager, wifiModule);
}

void Controllers::setReportController(Request& req, Response& res) noexcept {
    // Validate and set target parameters
    if (req.hasArg("ip") && req.hasArg("port") && req.hasArg("sp")) {
        String target_ip = req.arg("ip");
        String target_port = req.arg("port");
        int target_port_int = target_port.toInt();
        String target_sp = req.arg("sp");
        int target_sp_int = target_sp.toInt();

        // Set
        reporter.setNewReporting(target_ip, target_port_int, target_sp_int);

        logger.info("Target set to IP: %s, Port: %d, Sampling Period: %d", target_ip.c_str(), target_port_int,
                    target_sp_int);

        // Send confirmation
        res.send200();
    } else {
        // Send error response if required parameters are missing
        res.error("Missing required parameters: ip, port, sp");
    }
}

/**
 * @warning This controller returns the index.html
 */
void Controllers::rootController(Request& req, Response& res) noexcept {

    std::map<String, std::function<String()>> placeholderMap = {
        {"TEMPERATURE", [&]() { return String(sensorData.getTemperature()); }},
        {"LIGHT", [&]() { return String(sensorData.getLuminosity()); }},
        {"UPTIME", [&]() { return String(millis() / 1000); }},  // Uptime in seconds
        {"RUNNING", [&]() { return regulator.describeState(); }},
        {"FIRE", [&]() { return fireDetector.isFireDetected() ? "Yes" : "No"; }},
        {"SSID", [&]() { return wifiModule.getSSID(); }},
        {"MAC", [&]() { return wifiModule.getMAC(); }},
        {"IP", [&]() { return wifiModule.getIP(); }},
        {"COOLER", [&]() { return regulator.isCooling() ? "ON" : "OFF"; }},
        {"HEATER", [&]() { return regulator.isHeating() ? "ON" : "OFF"; }},
        {"FTT", [&]() { return String(fireDetector.getTempThreshold()); }},
        {"FLT", [&]() { return String(fireDetector.getLumThreshold()); }},
        {"LT", [&]() { return String(regulator.getLowThreshold()); }},
        {"HT", [&]() { return String(regulator.getHighThreshold()); }},
        {"PRT_IP", [&]() { return reporter.getTargetIP(); }},
        {"PRT_PORT", [&]() { return String(reporter.getTargetPort()); }},
        {"PRT_T", [&]() { return String(reporter.getTargetSP()); }}
    };

    // Send
    res.sendFile("/index.html",  [&](const String& var) { 
        auto it = placeholderMap.find(var);
        while(it != placeholderMap.end()) {
            return it->second();
        }
        return String();
    });
}