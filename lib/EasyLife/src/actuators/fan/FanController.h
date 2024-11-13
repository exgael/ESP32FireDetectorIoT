/*
 * Author: Benoît Barbier
 */

#pragma once

#include "../IActuator.h"

#include <Arduino.h>

class FanController : public IActuator {
   public:
    FanController(int pin, int pwmChannel);
    void initialize() noexcept;
    void turnOn() noexcept override;
    void turnOff() noexcept override;
    void setLevel(int level) noexcept;
    int getCurrentSpeed() const noexcept;

   private:
    int controlPin;
    int pwmChannel;
    int currentSpeed;
    Logger logger;
};
