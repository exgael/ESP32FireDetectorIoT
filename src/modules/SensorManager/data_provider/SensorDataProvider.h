/*
 * Author: Benoît Barbier
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

    // Get data
    const int& getLuminosity() const;
    const float& getTemperature() const;

    // Get historical data
    const std::deque<int>& getHistoricalLuminosity() const;
    const std::deque<float>& getHistoricalTemperature() const;

   private:

    // last reading
    int luminosity = 0;
    float temperature = 0.0;

    int buffer_size;
    std::deque<int> luminosityBuffer;
    std::deque<float> temperatureBuffer;
};
