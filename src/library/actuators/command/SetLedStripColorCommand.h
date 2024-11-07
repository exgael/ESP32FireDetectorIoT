/*
 * Author: Benoît Barbier
 * Created: 2024-11-07
 * Last Modified: 2024-11-07
 */


#pragma once

#include "../light/strip/LedStripController.h"
#include "../light/strip/color.h"
#include "ICommand.h"

class SetLedStripColorCommand : public ICommand {
   public:
    SetLedStripColorCommand(LedStripController* controller, int index, const Color& color)
        : controller(controller), index(index), color(color) {}

    void execute() override { controller->setColor(index, color); }

   private:
    LedStripController* controller;
    int index;
    Color color;
};
