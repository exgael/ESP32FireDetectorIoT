/*
 * Author: Benoît Barbier
 */

#pragma once

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
