/*
 * Author: Benoît Barbier
 */


#pragma once

#include <Arduino.h>

#include "../../logger/index.h"
#include "../ISensor.h"

class LightSensor : public ISensor {
   public:
    explicit LightSensor(int pin);
    void initialize() override;

    /**
     * @brief read the luminosity output from the light sensor.
     */
    float readData() override;

   private:
    int sensorPin;
    Logger logger;
};
