/*
 * Author: Benoît Barbier
 */


#pragma once

#include <Arduino.h>

#include "../ILedActuator.h"
#include "../led/LedController.h"

class OnboardLedController : public LedController {
   public:
    OnboardLedController();
};
