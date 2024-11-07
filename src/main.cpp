/*
 * Author: Benoît Barbier
 */


#include <Arduino.h>

#include "modules/ESPManager.h"

ESPManager* espManager = nullptr;

///////////////////////////////
//   Function declarations   //
///////////////////////////////

void createClock();
void createLogger();
ESPConfig& getConfig();
ActuatorManager* createControlModule(ESPConfig& config);
SensorManager* createSensorModule(ESPConfig& config);
FireDetection* createFireDetectionModule(SensorManager* sensorManager, ESPConfig& config);
TemperatureRegulator* createRegulationModule(ActuatorManager* actuatorManager, ESPConfig& config);
Reporter* createReportModule();
WiFiModule* createWiFiModule(ESPConfig& config);
ESPServer* createServerModule(SensorManager* sensorManager, FireDetection* fireDetector,
                              TemperatureRegulator* regulator, ActuatorManager* actuatorManager, Reporter* reporter,
                              WiFiModule* wifiModule, ESPConfig& config);
void createManager(ESPServer* server, SensorManager* sensorManager, FireDetection* fireDetector,
                   TemperatureRegulator* regulator, ActuatorManager* actuatorManager, Reporter* reporter,
                   WiFiModule* wifiModule);

///////////////////////////////
//           SETUP           //
///////////////////////////////

void setup() {

    // Initialize SPIFFS
    SPIFFS.begin(true);

    createClock();
    createLogger();
    
    ESPConfig& config = getConfig();

    // Initialize ESP Modules
    WiFiModule* wifiModule = createWiFiModule(config);
    wifiModule->init();
    ActuatorManager* actuatorManager = createControlModule(config);
    SensorManager* sensorManager = createSensorModule(config);
    FireDetection* fireDetector = createFireDetectionModule(sensorManager, config);
    TemperatureRegulator* regulator = createRegulationModule(actuatorManager, config);
    Reporter* reporter = createReportModule();

    ESPServer* server =
        createServerModule(sensorManager, fireDetector, regulator, actuatorManager, reporter, wifiModule, config);

    createManager(server, sensorManager, fireDetector, regulator, actuatorManager, reporter, wifiModule);
}

//////////////////////////////
//           LOOP           //
//////////////////////////////

void loop() { espManager->executeWorkflow(); }

//////////////////////////////
//   Function definitions   //
//////////////////////////////

void createClock() { Clock::sharedInstance().reset(); }

void createLogger() { Logger::begin(9600, LOG_LEVEL_DEBUG); }

ESPConfig& getConfig() { return ESPConfig::sharedInstance(); }

ActuatorManager* createControlModule(ESPConfig& config) {
    FanController* fanController = new FanController(config.FAN_PIN, config.FAN_PWM_CHANNEL);
    LedController* acLedController = new LedController(config.AC_LED_PIN);
    LedController* heaterLedController = new LedController(config.HEATER_LED_PIN);
    OnboardLedController* onboardLedController = new OnboardLedController();
    LedStripController* ledStripController = new LedStripController(config.LED_STRIP_PIN, config.LED_STRIP_SIZE);

    return new ActuatorManager(*fanController, *acLedController, *heaterLedController, *onboardLedController,
                               *ledStripController);
}

SensorManager* createSensorModule(ESPConfig& config) {
    TemperatureSensor* tempSensor = new TemperatureSensor(config.TEMP_SENSOR_PIN);
    LightSensor* lightSensor = new LightSensor(config.LIGHT_SENSOR_PIN);
    return new SensorManager(*tempSensor, *lightSensor, config.DETECTION_BUFFER_SIZE);
}

FireDetection* createFireDetectionModule(SensorManager* sensorManager, ESPConfig& config) {
    return new FireDetection(sensorManager->getSensorDataProvider(), config.FIRE_TEMPERATURE_THRESHOLD,
                             config.FIRE_LIGHT_THRESHOLD);
}

TemperatureRegulator* createRegulationModule(ActuatorManager* actuatorManager, ESPConfig& config) {
    return new TemperatureRegulator(*actuatorManager, config.LOW_TEMP, config.HIGH_TEMP);
}

Reporter* createReportModule() { return new Reporter(); }

WiFiModule* createWiFiModule(ESPConfig& config) { return new WiFiModule(config.NAME, config.SSID, config.PSSWD); }

ESPServer* createServerModule(SensorManager* sensorManager, FireDetection* fireDetector,
                              TemperatureRegulator* regulator, ActuatorManager* actuatorManager, Reporter* reporter,
                              WiFiModule* wifiModule, ESPConfig& config) {
    AsyncWebServer* asyncWebServer = new AsyncWebServer(config.PORT);
    return new ESPServer(*asyncWebServer, sensorManager->getSensorDataProvider(), *fireDetector, *regulator,
                         *actuatorManager, *reporter, *wifiModule);
}

void createManager(ESPServer* server, SensorManager* sensorManager, FireDetection* fireDetector,
                   TemperatureRegulator* regulator, ActuatorManager* actuatorManager, Reporter* reporter,
                   WiFiModule* wifiModule) {
    espManager =
        new ESPManager(*server, *sensorManager, *fireDetector, *regulator, *actuatorManager, *reporter, *wifiModule);
    espManager->init();
}
