/*
 * Author: Benoît Barbier
 */

#include "ESPManager.h"

#include <Arduino.h>

ESPManager *espManager = nullptr;

///////////////////////////////
//   Function declarations   //
///////////////////////////////

void createClock();
void createLogger();
ESPConfig &getConfig();
ActuatorManager *createControlModule(ESPConfig &config);
SensorManager *createSensorModule(ESPConfig &config);
Hotspot *createHotspot(SensorManager *sensorManager, ESPConfig &config);
WiFiModule *createWiFiModule(ESPConfig &config);
EasyServer *createServer(ESPConfig &config);
EasyMQTT *createMQTTPubSub(ESPConfig &config);
void createManager(
    EasyServer *server,
    EasyMQTT *mqttClient,
    SensorManager *sensorManager,
    Hotspot *hotspot,
    ActuatorManager *actuatorManager,
    WiFiModule *wifiModule);

///////////////////////////////
//           SETUP           //
///////////////////////////////

void setup()
{
    // Initialize SPIFFS
    SPIFFS.begin(true);

    createClock();
    createLogger();

    ESPConfig &config = getConfig();

    // Initialize ESP Modules
    WiFiModule *wifiModule = createWiFiModule(config);
    wifiModule->init();

    ActuatorManager *actuatorManager = createControlModule(config);
    SensorManager *sensorManager = createSensorModule(config);
    Hotspot *hotspot = createHotspot(sensorManager, config);
    EasyServer *server = createServer(config);
    EasyMQTT *mqttClient = createMQTTPubSub(config);

    createManager(
        server,
        mqttClient,
        sensorManager,
        hotspot,
        actuatorManager,
        wifiModule);
    espManager->init();
}

//////////////////////////////
//           LOOP           //
//////////////////////////////

void loop()
{
    espManager->executeWorkflow();
}

//////////////////////////////
//   Function definitions   //
//////////////////////////////

void createClock()
{
    Clock::sharedInstance().reset();
}

void createLogger()
{
    Logger::begin(9600, LOG_LEVEL_DEBUG);
}

ESPConfig &getConfig()
{
    return ESPConfig::sharedInstance();
}

ActuatorManager *createControlModule(ESPConfig &config)
{
    OnboardLedController *onboardLedController = new OnboardLedController();
    LedStripController *ledStripController =
        new LedStripController(config.LED_STRIP_PIN, config.LED_STRIP_SIZE);

    return new ActuatorManager(
        *onboardLedController,
        *ledStripController);
}

SensorManager *createSensorModule(ESPConfig &config)
{
    TemperatureSensor *tempSensor =
        new TemperatureSensor(config.TEMP_SENSOR_PIN);
    LightSensor *lightSensor = new LightSensor(config.LIGHT_SENSOR_PIN);
    return new SensorManager(
        *tempSensor, *lightSensor, config.DETECTION_BUFFER_SIZE);
}

Hotspot *createHotspot(SensorManager *sensorManager, ESPConfig &config)
{
    return new Hotspot(*sensorManager, config.LATITUDE, config.LONGITUDE);
}

WiFiModule *createWiFiModule(ESPConfig &config)
{
    return new WiFiModule(config.NAME, config.SSID, config.PSSWD);
}

EasyServer *createServer(ESPConfig &config)
{
    return new EasyServer(config.PORT);
}

EasyMQTT *createMQTTPubSub(ESPConfig &config)
{
    return new EasyMQTT(config.IDENT, config.MQTT_SERVER);
}

void createManager(
    EasyServer *server,
    EasyMQTT *mqttClient,
    SensorManager *sensorManager,
    Hotspot *hotspot,
    ActuatorManager *actuatorManager,
    WiFiModule *wifiModule)
{
    espManager = new ESPManager(
        *server,
        *mqttClient,
        *sensorManager,
        *hotspot,
        *actuatorManager,
        *wifiModule);
}
