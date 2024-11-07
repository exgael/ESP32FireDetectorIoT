/*
 * Author: Benoît Barbier
 * Created: 2024-11-07
 * Last Modified: 2024-11-07
 */


#pragma once

#include <Arduino.h>

#include "../../../logger/index.h"
#include "../ILedActuator.h"

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
