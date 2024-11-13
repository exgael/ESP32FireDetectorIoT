/*
 * Author: Benoît Barbier
 */

#include "FanController.h"

FanController::FanController(int pin, int pwmChannel)
    : controlPin(pin),
      pwmChannel(pwmChannel),
      currentSpeed(0),
      logger("FanController")
{
}

void FanController::initialize() noexcept
{
    ledcSetup(pwmChannel, 25000, 8); // 25 kHz frequency, 8-bit resolution
    ledcAttachPin(controlPin, pwmChannel);
    logger.debug(
        "FAN attach on pin %d for pwn channel %d, at 25 kHz on 8-bit "
        "resolutions.",
        controlPin,
        pwmChannel);
}

void FanController::turnOn() noexcept
{
    logger.debug("FAN ON.");
    currentSpeed = 128;
    setLevel(128); // Default speed
}

void FanController::turnOff() noexcept
{
    setLevel(0);
    currentSpeed = 0;
    logger.debug("FAN OFF.");
}

void FanController::setLevel(int level) noexcept
{
    currentSpeed = level;
    ledcWrite(pwmChannel, currentSpeed);
    logger.debug("FAN speed %d -> %d.", currentSpeed, level);
}

int FanController::getCurrentSpeed() const noexcept
{
    return currentSpeed;
}