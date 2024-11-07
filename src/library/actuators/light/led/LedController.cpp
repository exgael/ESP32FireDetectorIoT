/*
 * Author: Benoît Barbier
 * Created: 2024-11-07
 * Last Modified: 2024-11-07
 */


#include "LedController.h"

#include <Arduino.h>

LedController::LedController(int pin) : ledPin(pin), currentBrightness(255), logger("LedController") {}

void LedController::initialize() noexcept {
    // Set up
    pinMode(ledPin, OUTPUT);

    // Log
    logger.debug("Initialized LED on pin %d as OUTPUT.", ledPin);
}

void LedController::turnOn() noexcept {
    // Actuate
    digitalWrite(ledPin, currentBrightness);

    // Log
    logger.debug("LED on pin %d TURNED ON with brightness %d.", ledPin, currentBrightness);
}

void LedController::turnOff() noexcept {
    // Actuate
    digitalWrite(ledPin, LOW);

    // Log
    logger.debug("LED on pin %d TURNED OFF.", ledPin);
}

void LedController::setBrightness(int brightness) noexcept {
    // Validate
    brightness = constrain(brightness, 0, 255);

    // Save value
    currentBrightness = brightness;

    // Actuate
    digitalWrite(ledPin, brightness);

    // Log
    logger.debug("LED on pin %d, brightness %d -> %d.", ledPin, currentBrightness, brightness);
}
