/*
 * Author: Benoît Barbier
 */

#include "LightSensor.h"

LightSensor::LightSensor(int pin) : sensorPin(pin), logger("LightSensor")
{
}

void LightSensor::initialize()
{
    logger.debug("Light sensor on pin %d.", sensorPin);
}

float LightSensor::readData()
{
    auto data = analogRead(sensorPin);
    logger.debug("Reading light sensor on pin %d.", sensorPin);
    return static_cast<float>(data);
}
