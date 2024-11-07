/*
 * Author: Benoît Barbier
 * Created: 2024-11-07
 * Last Modified: 2024-11-07
 */


#pragma once

#include "../light/strip/LedStripController.h"
#include "../light/strip/color.h"
#include "ICommand.h"

class SetLedStripPatternCommand : public ICommand {
   public:
    SetLedStripPatternCommand(LedStripController* controller, const Color color, float percentage,
                              bool ascending = true)
        : controller(controller), color(color), percentage(percentage), ascending(ascending) {}

    void execute() override { controller->setGaugePattern(color, percentage, ascending); }

   private:
    LedStripController* controller;
    const Color color;
    const float percentage;
    const bool ascending;
};
