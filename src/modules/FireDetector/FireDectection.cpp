/*
 * Author: Benoît Barbier
 * Created: 2024-11-07
 * Last Modified: 2024-11-07
 */


#include "FireDetection.h"

FireDetection::FireDetection(const SensorDataProvider& data, float temperature_threshold, int luminosity_threshold,
                             int buffer_size)
    : data(data),
      temperature_threshold(temperature_threshold),
      luminosity_threshold(luminosity_threshold),
      logger("FireDetection") {}

const bool FireDetection::checkIfFireDetected() const noexcept {
    auto luminosityBuffer = data.getHistoricalLuminosity();
    auto temperatureBuffer = data.getHistoricalTemperature();

    int highLuminosityCount = countExceedances(luminosityBuffer, luminosity_threshold);
    int highTemperatureCount = countExceedances(temperatureBuffer, temperature_threshold);

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

void FireDetection::updateTemperatureThreshold(int new_threshold) noexcept { temperature_threshold = new_threshold; }

void FireDetection::updateLuminosityThreshold(int new_threshold) noexcept { luminosity_threshold = new_threshold; }

bool FireDetection::checkAlertCondition(int exceedCount, int buffer_size, float thresholdPercentage) const noexcept {
    return exceedCount >= buffer_size * thresholdPercentage / 100;
}

int FireDetection::countExceedances(const std::deque<int>& buffer, const float threshold) const noexcept {
    int count = 0;
    for (const auto& value : buffer) {
        if (threshold <= value) {
            count++;
        }
    }
    return count;
}

int FireDetection::countExceedances(const std::deque<float>& buffer, const int threshold) const noexcept {
    int count = 0;
    for (const auto& value : buffer) {
        if (threshold <= value) {
            count++;
        }
    }
    return count;
}

const bool FireDetection::isFireDetected() const noexcept { return fireDetectionLevel == 3; }
