/*
 * Author: Benoît Barbier
 */

#pragma once

#include "../IActuator.h"

class ILedActuator : public IActuator {
   public:
    virtual void setBrightness(int brightness) noexcept = 0;
    virtual ~ILedActuator() = default;
};
