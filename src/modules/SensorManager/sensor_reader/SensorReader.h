/*
 * Author: Benoît Barbier
 * Created: 2024-11-07
 * Last Modified: 2024-11-07
 */


#pragma once

#include "../../../library/index.h"
#include "../../../library/index.h"

class SensorReader {
   public:
    SensorReader(TemperatureSensor tempSensor, LightSensor lightSensor);
    int readLightSensor();
    float readTemperatureSensor();
    void init();

   private:
    TemperatureSensor tempSensor;
    LightSensor lightSensor;
    Logger logger;
};
