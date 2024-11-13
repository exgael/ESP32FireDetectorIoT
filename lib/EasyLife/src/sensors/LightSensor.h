/*
 * Author: Benoît Barbier
 */

#pragma once

#include "ISensor.h"

#include <Arduino.h>

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
