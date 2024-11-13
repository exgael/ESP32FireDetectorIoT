/*
 * Author: Benoît Barbier
 */

#pragma once

#include "../light/LedStripController.h"
#include "../light/Color.h"
#include "ICommand.h"

class SetLedStripColorCommand : public ICommand {
   public:
    SetLedStripColorCommand(LedStripController* controller, int index, const Color& color)
        : controller(controller), index(index), color(color), logger("SetLedStripColorCommand") {
        if (controller == nullptr) {
            throw std::invalid_argument("Controller cannot be null.");
        }
    }

    void execute() override {
        try {
            if (controller) {
                controller->setColor(index, color);
            }
        } catch (const std::exception& e) {
            logger.error("Failed to set LED color at index %d: %s", index, e.what());
        } catch (...) {
            logger.error("Unknown error occurred while setting LED color at index %d.", index);
        }
    }

   private:
    LedStripController* controller;
    int index;
    Color color;
    Logger logger;
};
