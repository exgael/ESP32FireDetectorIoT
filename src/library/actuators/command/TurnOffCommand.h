/*
 * Author: Benoît Barbier
 * Created: 2024-11-07
 * Last Modified: 2024-11-07
 */


#pragma once

#include "../IActuator.h"
#include "ICommand.h"

class TurnOffCommand : public ICommand {
   public:
    TurnOffCommand(IActuator* controller) : controller(controller) {}

    void execute() override { controller->turnOff(); }

   private:
    IActuator* controller;
};
