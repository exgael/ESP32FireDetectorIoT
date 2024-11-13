/*
 * Author: Benoît Barbier
 */

#include "TemperatureSensor.h"

TemperatureSensor::TemperatureSensor(int pin)
    : oneWire(pin),
      sensors(&oneWire),
      sensorPin(pin),
      logger("TemperatureSensor")
{
}

void TemperatureSensor::initialize()
{
    sensors.begin();
    logger.debug("Initialized via (oneWire) on pin %d.", sensorPin);
}

float TemperatureSensor::readData()
{
    sensors.requestTemperatures();
    logger.debug("Reading sensor on pin %d", sensorPin);
    return sensors.getTempCByIndex(0);
}
