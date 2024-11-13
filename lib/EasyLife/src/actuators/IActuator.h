/*
 * Author: Benoît Barbier
 */

#pragma once

// Since almost all actuators uses logger...
#include "../logger/index.h"

class IActuator {
   public:
    /**
     * @brief Turns on the targeted actuator.
     */
    virtual void turnOn() noexcept = 0;

    /**
     * @brief Turns off the targeted actuator.
     */
    virtual void turnOff() noexcept = 0;
    virtual ~IActuator() = default;
};
