/*
 * Author: Benoît Barbier
 */

#pragma once

#include "../light/Color.h"
#include "../light/LedStripController.h"
#include "ICommand.h"

class SetLedStripPatternCommand : public ICommand {
   public:
    SetLedStripPatternCommand(
        LedStripController *controller,
        const Color &color,
        float percentage,
        bool ascending = true)
        : controller(controller),
          color(color),
          percentage(percentage),
          ascending(ascending),
          logger("SetLedStripPatternCommand")
    {
        if (controller == nullptr) {
            throw std::invalid_argument("Controller cannot be null.");
        }
    }

    void execute() override
    {
        try {
            if (controller) {
                controller->setGaugePattern(color, percentage, ascending);
            }
        } catch (const std::exception &e) {
            logger.error("Failed to set LED strip pattern: %s", e.what());
        } catch (...) {
            logger.error(
                "Unknown error occurred while setting LED strip pattern.");
        }
    }

   private:
    LedStripController *controller;
    const Color color;
    const float percentage;
    const bool ascending;
    Logger logger;
};
