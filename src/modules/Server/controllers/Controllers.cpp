/*
 * Author: Benoît Barbier
 * Created: 2024-11-07
 * Last Modified: 2024-11-07
 */


#include "controllers.h"

#include "../../ESPConfig.h"
#include "../../Repoter/index.h"

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
            } else if (paramName == "lt") {
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

        reporter.setNewReporting(target_ip, target_port_int, target_sp_int);

        logger.info("Target set to IP: %s, Port: %d, Sampling Period: %d", target_ip.c_str(), target_port_int,
                    target_sp_int);

        // Send confirmation
        res.send200();
    } else {
        // Send error response if required parameters are missing
        res.error("{\"error\": \"Missing required parameters: ip, port, sp\"}");
    }
}

/**
 * @warning Need to SPIFFS it
 */
void Controllers::rootController(Request& req, Response& res) noexcept {
    String htmlFile = R"(<!DOCTYPE HTML>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <meta content="text/html;charset=utf-8" http-equiv="Content-Type">
  <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.7.2/css/all.min.css">
  <style>
    html {
        font-family: Arial;
        display: inline-block;
        margin: 0px auto;
        text-align: left;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .sensors-labels {
        font-size: 1.5rem;
        vertical-align: middle;
        padding-bottom: 15px;
    }
    div {
        max-width: 500px;
        word-wrap: break-word;
    }
    .grid-container {
        display: grid;
        grid-template-columns: auto auto;
        background-color: #992603;
        padding: 10px;
    }
    .grid-item {
        background-color: rgba(255, 255, 255, 0.8);
        border: 1px solid rgba(0, 0, 0, 0.8);
        padding: 20px;
        font-size: 30px;
        text-align: center;
    }
  </style>
  <title>ESP32 Dashboard</title>
</head>

<body>
  <h1>ESP32 Dashboard</h1>

  <h3>System Information:</h3>
  Uptime      : <span id="uptime">%UPTIME%</span> s<br/>
  Running     : <span id="running">%RUNNING%</span><br/>
  Fire Status : <span id="fire">%FIRE%</span><br/>

  <h3>Network Link Status:</h3>
  WiFi SSID   : %SSID%<br/>
  MAC Address : %MAC%<br/>
  IP Address  : %IP%<br/>

  <h3>Actuator Status:</h3>
  Cooler      : <span id="cooler">%COOLER%</span><br/>
  Heater      : <span id="heater">%HEATER%</span><br/>

  <h3>Thresholds:</h3>
  Fire Temp Threshold : <span id="FTT">%FTT%</span> °C<br/>
  Fire Light Threshold: <span id="FLT">%FLT%</span> <br/>
  Low Temp Threshold  : <span id="LT">%LT%</span> °C<br/>
  High Temp Threshold : <span id="HT">%HT%</span> °C<br/>

  <h3>Sensor Data with Icons:</h3>
  <div class="grid-container">
    <div class="grid-item">
      <i class="fas fa-thermometer-half" style="color:#059e8a;"></i>
    </div>
    <div class="grid-item">      
      <span class="sensors-labels">Temperature</span> 
      <span id="temperature">%TEMPERATURE%</span>
      <sup class="units">&deg;C</sup>
    </div>
    <div class="grid-item">
      <i class="far fa-lightbulb" style="color:#00add6;"></i>
    </div>
    <div class="grid-item">
      <span class="sensors-labels">Light</span>
      <span id="light">%LIGHT%</span>
      <sup class="units">Lumen</sup>
    </div>
  </div>

  <h3>Periodic Status Report Configuration:</h3>
  <form action="/target" method="post">
    <label for="ip">IP Address:</label>
    <input type="text" name="ip" placeholder="%PRT_IP%"/><br/>
    <label for="port">Port:</label>
    <input type="text" name="port" placeholder="%PRT_PORT%"/><br/>
    <label for="sp">Sampling Period (in seconds):</label>
    <input type="text" name="sp" placeholder="%PRT_T%"/><br/>
    <input type="submit" value="Change Reporting Host"/>
  </form>

  <script>
    function startUpdate() {
        setInterval(function() {
            fetch('/value?temperature&light&cooler&running&fire&uptime&FTT&FLT&LT&HT')
                .then(function(response) {
                    if (!response.ok) {
                        throw new Error(`HTTP error: ${response.status}`);
                    }
                    return response.json();
                })
                .then(function(data) {
                    for (let key in data) {
                        let element = document.getElementById(key);
                        if (element) {
                            element.innerHTML = data[key];
                        }
                    }
                })
                .catch(function(error) {
                    console.log(error);
                });
        }, 2000);
    }
    
    document.addEventListener('DOMContentLoaded', function() {
        startUpdate();
    });
  </script>
</body>
</html>)";

    // Send
    res.sendHtml(processor(htmlFile));
}

/**
 * @warning Need to SPIFFS it
 */
String Controllers::processor(String htmlFile) noexcept {
    struct Placeholder {
        const char* key;
        String value;
    };

    Placeholder placeholders[] = {                                        // System Information
                                  {"%UPTIME%", String(millis() / 1000)},  // Uptime in seconds
                                  {"%RUNNING%", regulator.describeState()},
                                  {"%FIRE%", fireDetector.isFireDetected() ? "Yes" : "No"},

                                  // Network Link Status
                                  {"%SSID%", wifiModule.getSSID()},
                                  {"%MAC%", wifiModule.getMAC()},
                                  {"%IP%", wifiModule.getIP()},

                                  // Sensor Status
                                  {"%TEMPERATURE%", String(sensorData.getTemperature())},
                                  {"%LIGHT%", String(sensorData.getLuminosity())},
                                  {"%COOLER%", regulator.isCooling() ? "ON" : "OFF"},
                                  {"%HEATER%", regulator.isHeating() ? "ON" : "OFF"},

                                  // Thresholds
                                  {"%FTT%", String(fireDetector.getTempThreshold())},
                                  {"%FLT%", String(fireDetector.getLumThreshold())},
                                  {"%LT%", String(regulator.getLowThreshold())},
                                  {"%HT%", String(regulator.getHighThreshold())},

                                  // Report status
                                  {"%PRT_IP%", reporter.getTargetIP()},
                                  {"%PRT_PORT%", String(reporter.getTargetPort())},
                                  {"%PRT_T%", String(reporter.getTargetSP())}};

    for (const auto& placeholder : placeholders) {
        htmlFile.replace(placeholder.key, placeholder.value);
    }

    return htmlFile;
}
