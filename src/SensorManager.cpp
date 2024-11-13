/*
 * Author: Benoît Barbier
 */

#include "SensorManager.h"

SensorManager::SensorManager(
    TemperatureSensor tempSensor, 
    LightSensor lightSensor, 
    int buffer_size
): 
    tempSensor(tempSensor), 
    lightSensor(lightSensor), 
    luminosityBuffer(buffer_size),
    temperatureBuffer(buffer_size), 
    buffer_size(buffer_size), 
    logger("SensorManager") {
}

void SensorManager::updateReadings() {
    // Perform actual sensor read
    int lum = readLightSensor();
    float temp = readTemperatureSensor();

    // Pass readings to storage
    setLuminosity(lum);
    setTemperature(temp);
}

void SensorManager::init() {
    lightSensor.initialize();
    tempSensor.initialize();
    logger.info("Sensors Ready.");
}

int SensorManager::readLightSensor() {
    int light = static_cast<int>(lightSensor.readData());
    logger.info("Light: %d", light);
    return light;
}

float SensorManager::readTemperatureSensor() {
    float temp = tempSensor.readData();
    logger.info("Temperature: %.2f°C.", temp);
    return temp;
}

const int& SensorManager::getLuminosity() const { return luminosity; }

const float& SensorManager::getTemperature() const { return temperature; }

const std::deque<int>& SensorManager::getHistoricalLuminosity() const { return luminosityBuffer; }

const std::deque<float>& SensorManager::getHistoricalTemperature() const { return temperatureBuffer; }

void SensorManager::setLuminosity(int lum) {
    luminosity = lum;
    if (luminosityBuffer.size() >= buffer_size) {
        luminosityBuffer.pop_front();
    }
    luminosityBuffer.push_back(lum);
}

void SensorManager::setTemperature(float temp) {
    temperature = temp;
    if (temperatureBuffer.size() >= buffer_size) {
        temperatureBuffer.pop_front();
    }
    temperatureBuffer.push_back(temp);
}
