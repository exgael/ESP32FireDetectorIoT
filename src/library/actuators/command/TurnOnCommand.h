/*
 * Author: Benoît Barbier
 * Created: 2024-11-07
 * Last Modified: 2024-11-07
 */


#pragma once

#include "../IActuator.h"
#include "ICommand.h"

class TurnOnCommand : public ICommand {
   public:
    TurnOnCommand(IActuator* controller) : controller(controller) {}
    void execute() override { controller->turnOn(); }

   private:
    IActuator* controller;
};
