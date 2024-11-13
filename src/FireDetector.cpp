/*
 * Author: Benoît Barbier
 */

#include "FireDetector.h"

FireDetector::FireDetector(
    const SensorManager& data, 
    float temperatureThreshold, 
    int luminosityThreshold,
    int buffer_size
):
    data(data),
    temperatureThreshold(temperatureThreshold),
    luminosityThreshold(luminosityThreshold),
    logger("FireDetector") {
}

const bool FireDetector::checkIfFireDetected() const noexcept {
    auto luminosityBuffer = data.getHistoricalLuminosity();
    auto temperatureBuffer = data.getHistoricalTemperature();

    int highLuminosityCount = countExceedances(luminosityBuffer, luminosityThreshold);
    int highTemperatureCount = countExceedances(temperatureBuffer, temperatureThreshold);

    bool luminosityAlert = checkAlertCondition(highLuminosityCount, luminosityBuffer.size());
    bool temperatureAlert = checkAlertCondition(highTemperatureCount, temperatureBuffer.size());

    if (luminosityAlert && temperatureAlert) {
        fireDetectionLevel = 3;
        logger.info("Fire detected.");
    } else if (temperatureAlert) {
        logger.info("Sustained temperature threshold crossed.");
        fireDetectionLevel = 2;
    } else if (luminosityAlert) {
        logger.info("Sustained light threshold crossed.");
        fireDetectionLevel = 1;
    } else {
        logger.info("No sustained threshold crossed.");
        fireDetectionLevel = 0;
    }

    return fireDetectionLevel == 3;
}

void FireDetector::updateTemperatureThreshold(
    int new_threshold
) noexcept { 
    temperatureThreshold = new_threshold; 
}

void FireDetector::updateLuminosityThreshold(
    int new_threshold
) noexcept { 
    luminosityThreshold = new_threshold; 
}

bool FireDetector::checkAlertCondition(
    int exceedCount, 
    int buffer_size, 
    float thresholdPercentage
) const noexcept {
    return exceedCount >= buffer_size * thresholdPercentage / 100;
}

const bool FireDetector::isFireDetected() const noexcept { 
    return fireDetectionLevel == 3; 
}