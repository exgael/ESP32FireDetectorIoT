/*
 * Author: Benoît Barbier
 */

#include "ESPManager.h"
#include "Server/Route.h"
#include <map>

ESPManager::ESPManager(
    EasyServer& server, 
    SensorManager& sensorManager, 
    FireDetector& fireDetector,
    TemperatureRegulator& regulator, 
    ActuatorManager& actuatorManager, 
    Reporter& reporter,
    WiFiModule& wifiModule
):
    server(server),
    sensorManager(sensorManager),
    fireDetector(fireDetector),
    regulator(regulator),
    actuatorManager(actuatorManager),
    reporter(reporter),
    wifiModule(wifiModule),
    logger("ESPManager") {
}

void ESPManager::init() {
    logger.info("");
    logger.info("*******   INIT   *******");
    logger.info("");

    sensorManager.init();
    actuatorManager.init();
    server.init();

    setupRouteHandlers(
        server,
        actuatorManager,
        sensorManager,
        regulator,
        fireDetector,
        wifiModule,
        reporter,
        logger
    );

    logger.info("");
    logger.info("******* ESP Ready *******");
    logger.info("");
}

void ESPManager::executeWorkflow() {
    Clock::sharedInstance().update();

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
    }

    if (executingMainPipeline == false) { // Protecting
        // Reporting
        reporter.handlePeriodicReporting(sensorManager, fireDetector, regulator, actuatorManager, wifiModule);
    }
}