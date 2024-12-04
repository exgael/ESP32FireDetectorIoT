/*
 * Author: Benoît Barbier
 */

#include "ESPManager.h"
#include "MQTT/Callback.h"
#include "Server/Route.h"

#include <map>

ESPManager::ESPManager(
    EasyServer &server,
    EasyMQTT &mqttClient,
    SensorManager &sensorManager,
    FireDetector &fireDetector,
    TemperatureRegulator &regulator,
    AmIHotspot &hotspot,
    ActuatorManager &actuatorManager,
    WiFiModule &wifiModule)
    : server(server),
      mqttClient(mqttClient),
      sensorManager(sensorManager),
      fireDetector(fireDetector),
      regulator(regulator),
      hotspot(hotspot),
      actuatorManager(actuatorManager),
      wifiModule(wifiModule),
      logger("ESPManager")
{
}

void ESPManager::init()
{
    logger.info("");
    logger.info("*******   INIT   *******");
    logger.info("");

    sensorManager.init();
    actuatorManager.init();
    server.init();
    mqttClient.init();

    setupRouteHandlers(
        server,
        actuatorManager,
        sensorManager,
        regulator,
        fireDetector,
        wifiModule,
        logger);

    setupCallback(mqttClient, hotspot, logger);

    logger.info("");
    logger.info("******* ESP Ready *******");
    logger.info("");
}

void ESPManager::executeWorkflow()
{
    // Runs every time
    Clock::sharedInstance().update();
    mqttClient.subscribe(ESPConfig::sharedInstance().TOPIC_TEMP, 0);

    // Intervaled
    if (Clock::sharedInstance().hasTimePassed(tick, interval)) {
        // Main
        if (iter % 5 == 0) {
            logger.info("");
            logger.info("*******  %lu  *******", iter);
            logger.info("");

            sensorManager.updateReadings();
            const bool isFireDetected = fireDetector.checkIfFireDetected();
            regulator.regulate(sensorManager, isFireDetected);
            actuatorManager.processCommands();

            // Publish
            String payload =
                PayloadMaker::getPiscineStateString(sensorManager, hotspot);
            mqttClient.publish(ESPConfig::sharedInstance().TOPIC_TEMP, payload);
        }

        // Wifi status
        if (iter % 10 == 0) {
            wifiModule.printStatus();
        }

        // Process incoming msg
        mqttClient.loop();

        // Log HotSpot Status
        if (iter % 5 == 0) {
            logger.debug(hotspot.toString().c_str());
        }

        // New iteration
        ++iter;
    }
}