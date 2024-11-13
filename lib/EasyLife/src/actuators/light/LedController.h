/*
 * Author: Benoît Barbier
 */

#pragma once

#include "ILedActuator.h"

#include <Arduino.h>

class LedController : public ILedActuator {
   public:
    explicit LedController(int pin);
    void initialize() noexcept;
    void turnOn() noexcept override;
    void turnOff() noexcept override;
    void setBrightness(int brightness) noexcept override;

   private:
    int ledPin;
    int currentBrightness;
    Logger logger;
};
