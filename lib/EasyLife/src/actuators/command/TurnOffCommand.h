/*
 * Author: Benoît Barbier
 */

#pragma once

#include "../IActuator.h"
#include "ICommand.h"

class TurnOffCommand : public ICommand {
   public:
    TurnOffCommand(IActuator* controller) : controller(controller), logger("TurnOffCommand") {
        if (controller == nullptr) {
            throw std::invalid_argument("Controller cannot be null.");
        }
    }

    void execute() override {
        try {
            if (controller) {
                controller->turnOff();
            }
        } catch (const std::exception& e) {
            logger.error("Failed to turn off actuator: %s", std::string(e.what()));
        } catch (...) {
            logger.error("Unknown error occurred while turning off actuator.");
        }
    }

   private:
    IActuator* controller;
    Logger logger;
};
