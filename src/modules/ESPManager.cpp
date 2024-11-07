/*
 * Author: Benoît Barbier
 * Created: 2024-11-07
 * Last Modified: 2024-11-07
 */


#include "ESPManager.h"

ESPManager::ESPManager(ESPServer& server, SensorManager& sensorManager, FireDetection& fireDetector,
                       TemperatureRegulator& regulator, ActuatorManager& actuatorManager, Reporter& reporter,
                       WiFiModule& wifiModule)
    : server(server),
      sensorManager(sensorManager),
      fireDetector(fireDetector),
      regulator(regulator),
      actuatorManager(actuatorManager),
      reporter(reporter),
      wifiModule(wifiModule),
      logger("ESPManager") {}

void ESPManager::init() {
    logger.info("");
    logger.info("*******   INIT   *******");
    logger.info("");

    sensorManager.init();
    actuatorManager.init();
    server.init();

    logger.info("");
    logger.info("******* ESP Ready *******");
    logger.info("");
}

void ESPManager::executeWorkflow() {
    Clock::sharedInstance().update();

    if (Clock::sharedInstance().hasTimePassed(tick, interval)) {
        logger.info("");
        logger.info("*******  %lu  *******", iter);
        logger.info("");

        if (iter % 10 == 0) {
            wifiModule.printStatus();
        }

        sensorManager.updateReadings();
        const bool isFireDetected = fireDetector.checkIfFireDetected();
        regulator.regulate(sensorManager.getSensorDataProvider(), isFireDetected);
        actuatorManager.processCommands();

        // Increment main pipeline iteration count
        iter++;
    }

    // Reporting
    server.periodicRun();
}
