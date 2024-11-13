/*
 * Author: Benoît Barbier
 */

#include "LedController.h"

#include <Arduino.h>

LedController::LedController(int pin)
    : ledPin(pin), currentBrightness(255), logger("LedController")
{
}

void LedController::initialize() noexcept
{
    pinMode(ledPin, OUTPUT);
    logger.debug("Initialized LED on pin %d as OUTPUT.", ledPin);
}

void LedController::turnOn() noexcept
{
    digitalWrite(ledPin, currentBrightness);
    logger.debug(
        "LED on pin %d TURNED ON with brightness %d.",
        ledPin,
        currentBrightness);
}

void LedController::turnOff() noexcept
{
    digitalWrite(ledPin, LOW);
    logger.debug("LED on pin %d TURNED OFF.", ledPin);
}

void LedController::setBrightness(int brightness) noexcept
{
    brightness = constrain(brightness, 0, 255);
    currentBrightness = brightness;
    digitalWrite(ledPin, brightness);
    logger.debug(
        "LED on pin %d, brightness %d -> %d.",
        ledPin,
        currentBrightness,
        brightness);
}
