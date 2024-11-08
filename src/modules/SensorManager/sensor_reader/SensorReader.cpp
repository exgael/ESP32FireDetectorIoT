/*
 * Author: Benoît Barbier
 */

#include "SensorReader.h"

SensorReader::SensorReader(TemperatureSensor tempSensor, LightSensor lightSensor)
    : tempSensor(tempSensor), lightSensor(lightSensor), logger("SensorReader") {}

int SensorReader::readLightSensor() {
    int light = static_cast<int>(lightSensor.readData());
    logger.info("Light: %d", light);
    return light;
}

float SensorReader::readTemperatureSensor() {
    float temp = tempSensor.readData();
    logger.info("Temperature: %.2f°C.", temp);
    return temp;
}

void SensorReader::init() {
    lightSensor.initialize();
    tempSensor.initialize();
}
