/*
 * Author: Benoît Barbier
 * Created: 2024-11-07
 * Last Modified: 2024-11-07
 */


#include "TemperatureRegulator.h"

TemperatureRegulator::TemperatureRegulator(ActuatorManager& actuatorManager, float lowerBound, float upperBound)
    : actuatorManager(actuatorManager),
      lowerBound(lowerBound),
      upperBound(upperBound),
      current_state(State::HALT),
      logger("TemperatureRegulator") {}

void TemperatureRegulator::regulate(const SensorDataProvider& data, const bool isFireDetected) {
    float temperature = data.getTemperature();

    // Handle emergency halt
    if (isFireDetected == true) {
        if (current_state != State::EMERGENCY_HALT) {
            current_state = State::EMERGENCY_HALT;
            logger.debug("State changed to EMERGENCY_HALT.");
            emergencyHalt();
        }
        return;
    } else if (current_state == State::EMERGENCY_HALT) {
        // Exit emergency halt when safe
        current_state = State::HALT;
        actuatorManager.requestOnboardLedOff();
        logger.debug("State changed EMERGENCY_HALT -> HALT.");
    }

    // State transitions based on temperature
    if (temperature < lowerBound) {
        if (current_state != State::HEATING) {
            current_state = State::HEATING;
            logger.debug("State changed to HEATING.");
        }
        heat(temperature);
    } else if (temperature > upperBound) {
        if (current_state != State::COOLING) {
            current_state = State::COOLING;
            logger.debug("State changed to COOLING.");
        }
        cool(temperature);
    } else {
        if (current_state != State::HALT) {
            current_state = State::HALT;
            logger.debug("State changed to HALT.");
        }
        halt(temperature);
    }
}

String TemperatureRegulator::describeState() const {
    return (current_state == State::HEATING || current_state == State::COOLING) ? "RUNNING" : "HALT";
}

void TemperatureRegulator::cool(const float temperature) {
    int fanLevel = map(temperature, upperBound, upperBound + 5, 192, 255);
    fanLevel = constrain(fanLevel, 192, 255);

    actuatorManager.requestFanSpeed(fanLevel);
    actuatorManager.requestFanOn();
    actuatorManager.requestCoolerOn();
    actuatorManager.requestHeaterOff();
    actuatorManager.requestOnboardLedOff();

    int ledStripLevel = map(temperature, upperBound, upperBound + 5, 1, 100);
    ledStripLevel = constrain(ledStripLevel, 1, 100);
    actuatorManager.requestLedStripRed(ledStripLevel);
}

void TemperatureRegulator::heat(const float temperature) {
    actuatorManager.requestFanOff();
    actuatorManager.requestCoolerOff();
    actuatorManager.requestHeaterOn();
    actuatorManager.requestOnboardLedOff();

    int ledStripLevel = map(temperature, lowerBound - 5, lowerBound, 1, 100);
    ledStripLevel = constrain(ledStripLevel, 1, 100);
    actuatorManager.requestLedStripGreen(ledStripLevel);
}

void TemperatureRegulator::halt(const float temperature) {
    actuatorManager.requestFanOff();
    actuatorManager.requestCoolerOff();
    actuatorManager.requestHeaterOff();

    int ledStripLevel = map(temperature, lowerBound, upperBound, 1, 100);
    ledStripLevel = constrain(ledStripLevel, 1, 100);
    actuatorManager.requestLedStripOrange(ledStripLevel);
}

void TemperatureRegulator::emergencyHalt() {
    actuatorManager.requestFanOff();
    actuatorManager.requestCoolerOff();
    actuatorManager.requestHeaterOff();

    actuatorManager.requestOnboardLedOn();
    actuatorManager.requestLedStripRed(5);
}

bool TemperatureRegulator::isCooling() const noexcept { return current_state == State::COOLING; }

bool TemperatureRegulator::isHeating() const noexcept { return current_state == State::HEATING; }

int TemperatureRegulator::getLowThreshold() const noexcept { return lowerBound; }
int TemperatureRegulator::getHighThreshold() const noexcept { return upperBound; }
void TemperatureRegulator::setLowThreshold(const float newThreshold) noexcept {
    if (newThreshold < upperBound) {
        lowerBound = newThreshold;
    }
}
void TemperatureRegulator::setHighThreshold(const float newThreshold) noexcept {
    if (newThreshold < lowerBound) {
        upperBound = newThreshold;
    }
}
