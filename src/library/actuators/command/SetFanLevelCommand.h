/*
 * Author: Benoît Barbier
 * Created: 2024-11-07
 * Last Modified: 2024-11-07
 */


#pragma once

#include "../fan/FanController.h"
#include "ICommand.h"

class SetFanLevelCommand : public ICommand {
   public:
    SetFanLevelCommand(FanController* controller, int level) : controller(controller), level(level) {}

    void execute() override { controller->setLevel(level); }

   private:
    FanController* controller;
    int level;
};
