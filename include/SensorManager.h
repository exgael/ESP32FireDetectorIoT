/*
 * Author: Benoît Barbier
 */

#pragma once

#include <EasyLife.h>
#include <deque>

class SensorManager {
   public:
    SensorManager(TemperatureSensor tempSensor, LightSensor lightSensor, int buffer_size);

    /**
     * @brief Read sensors and store readings in in-memory storage.
     */
    void updateReadings();

    void init();

    // Get data
    const int& getLuminosity() const;
    const float& getTemperature() const;

    // Get historical data
    const std::deque<int>& getHistoricalLuminosity() const;
    const std::deque<float>& getHistoricalTemperature() const;

   private:
    TemperatureSensor tempSensor;
    LightSensor lightSensor;

    // last reading
    int luminosity = 0;
    float temperature = 0.0;

    int buffer_size;
    std::deque<int> luminosityBuffer;
    std::deque<float> temperatureBuffer;

    // Update data
    int readLightSensor();
    float readTemperatureSensor();
    void setLuminosity(int lum);
    void setTemperature(float temp);

    Logger logger;
};