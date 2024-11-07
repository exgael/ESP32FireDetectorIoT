/*
 * Author: Benoît Barbier
 * Created: 2024-11-07
 * Last Modified: 2024-11-07
 */


#pragma once

#include "../light/ILedActuator.h"
#include "ICommand.h"

class SetBrightnessCommand : public ICommand {
   public:
    SetBrightnessCommand(ILedActuator* controller, int brightness) : controller(controller), brightness(brightness) {}

    void execute() override { controller->setBrightness(brightness); }

   private:
    ILedActuator* controller;
    int brightness;
};
