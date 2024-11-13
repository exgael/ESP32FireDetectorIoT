/*
 * Author: Benoît Barbier
 */

#pragma once

#include "../fan/FanController.h"
#include "ICommand.h"

class SetFanLevelCommand : public ICommand {
   public:
    SetFanLevelCommand(FanController* controller, int level)
        : controller(controller), level(level), logger("SetFanLevelCommand") {
        if (controller == nullptr) {
            throw std::invalid_argument("Controller cannot be null.");
        }
    }

    void execute() override {
        try {
            if (controller) {
                controller->setLevel(level);
            }
        } catch (const std::exception& e) {
            logger.error("Failed to set fan level: %s", e.what());
        } catch (...) {
            logger.error("Unknown error occurred while setting fan level.");
        }
    }

   private:
    FanController* controller;
    int level;
    Logger logger;
};
