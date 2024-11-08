/*
 * Author: Benoît Barbier
 */

#include "SensorManager.h"

SensorManager::SensorManager(TemperatureSensor tempSensor, LightSensor lightSensor, int buffer_size)
    : sensor_reader(tempSensor, lightSensor), sensor_data_provider(buffer_size), logger("SensorManager") {}

const SensorDataProvider& SensorManager::getSensorDataProvider() const { return sensor_data_provider; }

void SensorManager::updateReadings() {
    // Perform actual sensor read
    int lum = sensor_reader.readLightSensor();
    float temp = sensor_reader.readTemperatureSensor();

    // Pass readings to storage
    sensor_data_provider.setLuminosity(lum);
    sensor_data_provider.setTemperature(temp);
}

void SensorManager::init() {
    sensor_reader.init();
    logger.info("Sensors Ready.");
}
