/*
 * Author: Benoît Barbier
 * Created: 2024-11-07
 * Last Modified: 2024-11-07
 */


#pragma once

#include "../../library/index.h"
#include "../SensorManager/index.h"
#include "../../library/index.h"

class FireDetection {
   public:
    FireDetection(const SensorDataProvider& data, float temperature_threshold, int luminosity_threshold,
                  int buffer_size = 10);

    /**
     * @brief Analyzes sensor data to detect fire or potential risks and updates
     * `fireDetectionLevel`.
     *
     * This procedure evaluates data from luminosity and temperature sensors to
     * determine if a fire is detected or if any thresholds are exceeded.
     */
    const bool checkIfFireDetected() const noexcept;

    /**
     * @brief Updates the temperature threshold value.
     */
    void updateTemperatureThreshold(int new_threshold) noexcept;

    /**
     * @brief Updates the luminosity threshold value.
     */
    void updateLuminosityThreshold(int new_threshold) noexcept;

    /**
     * @brief Getter for fire detected
     * @warning Does not do anything but returning a boolean.
     * @return bool: `true` if fire detected, `false` otherwise.
     */
    const bool isFireDetected() const noexcept;

    int getLumThreshold() { return luminosity_threshold; }
    int getTempThreshold() { return temperature_threshold; }

   private:
    /**
     * @brief int Detection level:
     *
     *  - 3: Fire detected (both temperature and luminosity exceed thresholds)
     *
     *  - 2: Sustained temperature threshold exceedance
     *
     *  - 1: Sustained luminosity threshold exceedance
     *
     *  - 0: No threshold exceedance
     */
    mutable int fireDetectionLevel;  // Might be usefull if i want to show more details

    const SensorDataProvider& data;
    float temperature_threshold;
    int luminosity_threshold;

    bool checkAlertCondition(int exceedCount, int buffer_size, float thresholdPercentage = 70.0) const noexcept;
    int countExceedances(const std::deque<int>& buffer, const float threshold) const noexcept;
    int countExceedances(const std::deque<float>& buffer, const int threshold) const noexcept;

    Logger logger;
};
