/*
 * Author: Benoît Barbier
 */

#include "FireDetection.h"

FireDetection::FireDetection(const SensorDataProvider& data, float temperatureThreshold, int luminosityThreshold,
                             int buffer_size)
    : data(data),
      temperatureThreshold(temperatureThreshold),
      luminosityThreshold(luminosityThreshold),
      logger("FireDetection") {}

const bool FireDetection::checkIfFireDetected() const noexcept {
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

void FireDetection::updateTemperatureThreshold(int new_threshold) noexcept { temperatureThreshold = new_threshold; }

void FireDetection::updateLuminosityThreshold(int new_threshold) noexcept { luminosityThreshold = new_threshold; }

bool FireDetection::checkAlertCondition(int exceedCount, int buffer_size, float thresholdPercentage) const noexcept {
    return exceedCount >= buffer_size * thresholdPercentage / 100;
}

const bool FireDetection::isFireDetected() const noexcept { return fireDetectionLevel == 3; }