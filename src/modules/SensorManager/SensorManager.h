/*
 * Author: Benoît Barbier
 * Created: 2024-11-07
 * Last Modified: 2024-11-07
 */


#pragma once

#include "../../library/index.h"
#include "data_provider/index.h"
#include "sensor_reader/index.h"

class SensorManager {
   public:
    SensorManager(TemperatureSensor tempSensor, LightSensor lightSensor, int buffer_size);

    // read-only access
    const SensorDataProvider& getSensorDataProvider() const;

    // Update
    void updateReadings();

    void init();

   private:
    SensorReader sensor_reader;
    SensorDataProvider sensor_data_provider;
    Logger logger;
};
