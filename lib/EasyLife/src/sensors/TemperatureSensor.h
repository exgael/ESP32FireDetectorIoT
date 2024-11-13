/*
 * Author: Benoît Barbier
 */

#pragma once

#include <DallasTemperature.h>
#include <OneWire.h>

#include "ISensor.h"

class TemperatureSensor : public ISensor {
   public:
    explicit TemperatureSensor(int pin);
    void initialize() override;

    /**
     * @brief read the temperature output from the temperature sensor.
     */
    float readData() override;

   private:
    OneWire oneWire;
    DallasTemperature sensors;
    int sensorPin;
    Logger logger;
};
