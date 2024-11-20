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
    Reporter &reporter,
    WiFiModule &wifiModule)
    : server(server),
      mqttClient(mqttClient),
      sensorManager(sensorManager),
      fireDetector(fireDetector),
      regulator(regulator),
      hotspot(hotspot),
      actuatorManager(actuatorManager),
      reporter(reporter),
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
        reporter,
        logger);

    setupCallback(mqttClient, hotspot, logger);

    logger.info("");
    logger.info("******* ESP Ready *******");
    logger.info("");
}

void ESPManager::executeWorkflow()
{
    Clock::sharedInstance().update();

    mqttClient.subscribe(ESPConfig::sharedInstance().TOPIC_TEMP, 0);

    if (Clock::sharedInstance().hasTimePassed(tick, interval)) {
        executingMainPipeline = true;
        logger.info("");
        logger.info("*******  %lu  *******", iter);
        logger.info("");

        if (iter % 10 == 0) {
            wifiModule.printStatus();
        }

        sensorManager.updateReadings();
        const bool isFireDetected = fireDetector.checkIfFireDetected();
        regulator.regulate(sensorManager, isFireDetected);
        actuatorManager.processCommands();

        // Increment main pipeline iteration count
        iter++;
        executingMainPipeline = false;

        String payload =
            PayloadMaker::getPiscineStateString(sensorManager, hotspot);

        mqttClient.publish(ESPConfig::sharedInstance().TOPIC_TEMP, payload);
        mqttClient.loop();

        if (iter % 10 == 0) {
            logger.debug(hotspot.toString().c_str());
        }
    }

    // if (executingMainPipeline == false) { // Protecting
    //     // Reporting
    //     reporter.handlePeriodicReporting(
    //         sensorManager,
    //         fireDetector,
    //         regulator,
    //         actuatorManager,
    //         wifiModule);
    // }
}