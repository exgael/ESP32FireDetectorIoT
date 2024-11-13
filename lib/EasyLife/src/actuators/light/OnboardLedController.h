/*
 * Author: Benoît Barbier
 */

#pragma once

#include <Arduino.h>

#include "ILedActuator.h"
#include "LedController.h"

class OnboardLedController : public LedController {
   public:
    OnboardLedController();
};
