/*
 * Author: Benoît Barbier
 */


#pragma once

#include "../../library/index.h"
#include "data_provider/index.h"
#include "sensor_reader/index.h"

class SensorManager {
   public:
    SensorManager(TemperatureSensor tempSensor, LightSensor lightSensor, int buffer_size);

    /**
     * @brief Get the class handling data in-memory storage.
     */
    const SensorDataProvider& getSensorDataProvider() const;

    /**
     * @brief Read sensors and store readings in in-memory storage.
     */
    void updateReadings();

    void init();

   private:
    SensorReader sensor_reader;
    SensorDataProvider sensor_data_provider;
    Logger logger;
};
