/*
 * Author: Benoît Barbier
 */

#pragma once

#include "ILedActuator.h"
#include "LedController.h"

#include <Arduino.h>

class OnboardLedController : public LedController {
   public:
    OnboardLedController();
};
