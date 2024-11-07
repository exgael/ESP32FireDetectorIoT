/*
 * Author: Benoît Barbier
 */


#include "SensorDataProvider.h"

SensorDataProvider::SensorDataProvider(int buffer_size)
    : luminosityBuffer(buffer_size), temperatureBuffer(buffer_size), buffer_size(buffer_size) {}

const int& SensorDataProvider::getLuminosity() const { return luminosity; }

const float& SensorDataProvider::getTemperature() const { return temperature; }

const std::deque<int>& SensorDataProvider::getHistoricalLuminosity() const { return luminosityBuffer; }

const std::deque<float>& SensorDataProvider::getHistoricalTemperature() const { return temperatureBuffer; }

void SensorDataProvider::setLuminosity(int lum) {
    luminosity = lum;
    if (luminosityBuffer.size() >= buffer_size) {
        luminosityBuffer.pop_front();
    }
    luminosityBuffer.push_back(lum);
}

void SensorDataProvider::setTemperature(float temp) {
    temperature = temp;
    if (temperatureBuffer.size() >= buffer_size) {
        temperatureBuffer.pop_front();
    }
    temperatureBuffer.push_back(temp);
}
