/*
 * Author: Benoît Barbier
 * Created: 2024-11-07
 * Last Modified: 2024-11-07
 */


#pragma once

#include <DallasTemperature.h>
#include <OneWire.h>

#include "../../logger/index.h"
#include "../ISensor.h"

class TemperatureSensor : public ISensor {
   public:
    explicit TemperatureSensor(int pin);
    void initialize() override;
    float readData() override;

   private:
    OneWire oneWire;
    DallasTemperature sensors;

    // For logging purposes.
    int sensorPin;
    Logger logger;
};
