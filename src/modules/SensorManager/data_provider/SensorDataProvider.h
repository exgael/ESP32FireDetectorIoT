/*
 * Author: Benoît Barbier
 * Created: 2024-11-07
 * Last Modified: 2024-11-07
 */


#pragma once

#include <deque>

/**
 * @brief Provider
 */
class SensorDataProvider {
   public:
    SensorDataProvider(int buffer_size);

    // Update data
    void setLuminosity(int lum);
    void setTemperature(float temp);

    // Read-only access to current readings
    const int& getLuminosity() const;
    const float& getTemperature() const;

    // Read-only access to historical data
    const std::deque<int>& getHistoricalLuminosity() const;
    const std::deque<float>& getHistoricalTemperature() const;

   private:
    int luminosity = 0;
    float temperature = 0.0;

    int buffer_size;
    std::deque<int> luminosityBuffer;
    std::deque<float> temperatureBuffer;
};
