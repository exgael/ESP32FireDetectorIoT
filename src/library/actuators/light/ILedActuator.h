/*
 * Author: Benoît Barbier
 * Created: 2024-11-07
 * Last Modified: 2024-11-07
 */


#pragma once

#include "../IActuator.h"

class ILedActuator : public IActuator {
   public:
    virtual void setBrightness(int brightness) noexcept = 0;
    virtual ~ILedActuator() = default;
};
