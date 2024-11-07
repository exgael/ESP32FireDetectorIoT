/*
 * Author: Benoît Barbier
 * Created: 2024-11-07
 * Last Modified: 2024-11-07
 */


#pragma once

#include <Arduino.h>

#include "../../logger/index.h"
#include "../ISensor.h"

class LightSensor : public ISensor {
   public:
    explicit LightSensor(int pin);
    void initialize() override;
    float readData() override;

   private:
    int sensorPin;
    Logger logger;
};
