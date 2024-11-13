/*
 * Author: Benoît Barbier
 */

#pragma once

#include "../IActuator.h"
#include "ICommand.h"

class TurnOnCommand : public ICommand {
   public:
    TurnOnCommand(IActuator *controller)
        : controller(controller), logger("TurnOnCommand")
    {
        if (controller == nullptr) {
            throw std::invalid_argument("Controller cannot be null.");
        }
    }

    void execute() override
    {
        try {
            if (controller) {
                controller->turnOn();
            }
        } catch (const std::exception &e) {
            logger.error(
                "Failed to turn on actuator: %s", std::string(e.what()));
        } catch (...) {
            logger.error("Unknown error occurred while turning on actuator.");
        }
    }

   private:
    IActuator *controller;
    Logger logger;
};
