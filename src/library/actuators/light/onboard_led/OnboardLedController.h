/*
 * Author: Benoît Barbier
 * Created: 2024-11-07
 * Last Modified: 2024-11-07
 */


#pragma once

#include <Arduino.h>

#include "../ILedActuator.h"
#include "../led/LedController.h"

class OnboardLedController : public LedController {
   public:
    OnboardLedController();
};
