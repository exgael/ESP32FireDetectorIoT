/*
 * Author: Benoît Barbier
 */

#pragma once

#include "../light/ILedActuator.h"
#include "ICommand.h"

class SetBrightnessCommand : public ICommand {
   public:
    SetBrightnessCommand(ILedActuator *controller, int brightness)
        : controller(controller),
          brightness(brightness),
          logger("SetBrightnessCommand")
    {
        if (controller == nullptr) {
            throw std::invalid_argument("Controller cannot be null.");
        }
    }

    void execute() override
    {
        try {
            if (controller) {
                controller->setBrightness(brightness);
            }
        } catch (const std::exception &e) {
            logger.error("Failed to set brightness: %s", e.what());
        } catch (...) {
            logger.error("Unknown error occurred while setting brightness.");
        }
    }

   private:
    ILedActuator *controller;
    int brightness;
    Logger logger;
};
