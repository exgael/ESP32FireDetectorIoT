/*
 * Author: Benoît Barbier
 */

#include "ActuatorManager.h"

ActuatorManager::ActuatorManager(
    FanController fanController, 
    LedController coolerController,
    LedController heaterController, 
    OnboardLedController fireAlarmController,
    LedStripController ledStripController
): 
    fanController(fanController),
    coolerController(coolerController),
    heaterController(heaterController),
    fireAlarmController(fireAlarmController),
    ledStripController(ledStripController),
    logger("ActuatorManager") {
}

// Fan Control
void ActuatorManager::requestFanOn() noexcept {
    if (!isFanOn) {
        logger.debug("CMD FAN ON accepted.");
        addCommand(new TurnOnCommand(&fanController));
        isFanOn = true;
    }
}

void ActuatorManager::requestFanOff() noexcept {
    if (isFanOn) {
        logger.debug("CMD FAN OFF accepted.");
        addCommand(new TurnOffCommand(&fanController));
        isFanOn = false;
    }
}

void ActuatorManager::requestFanSpeed(float level) noexcept {
    if (fanController.getCurrentSpeed() != static_cast<int>(level)) {
        logger.debug("CMD FAN SPEED %.2f accepted.", level);
        addCommand(new SetFanLevelCommand(&fanController, level));
    }
}

// Cooler Control
void ActuatorManager::requestCoolerOn() noexcept {
    if (!isCoolerOn) {
        logger.debug("CMD COOLER ON accepted.");
        addCommand(new TurnOnCommand(&coolerController));
        isCoolerOn = true;
    }
}

void ActuatorManager::requestCoolerOff() noexcept {
    if (isCoolerOn) {
        logger.debug("CMD COOLER OFF accepted.");
        addCommand(new TurnOffCommand(&coolerController));
        isCoolerOn = false;
    }
}

// Heater Control
void ActuatorManager::requestHeaterOn() noexcept {
    if (!isHeaterOn) {
        logger.debug("CMD HEATER ON at accepted.");
        addCommand(new TurnOnCommand(&heaterController));
        isHeaterOn = true;
    }
}

void ActuatorManager::requestHeaterOff() noexcept {
    if (isHeaterOn) {
        logger.debug("CMD HEATER OFF at accepted.");
        addCommand(new TurnOffCommand(&heaterController));
        isHeaterOn = false;
    }
}

// Onboard LED Control
void ActuatorManager::requestOnboardLedOn() noexcept {
    if (!isOnboardLedOn) {
        logger.debug("CMD ONBOARD_LED ON accepted.");
        addCommand(new TurnOnCommand(&fireAlarmController));
        isOnboardLedOn = true;
    }
}

void ActuatorManager::requestOnboardLedOff() noexcept {
    if (isOnboardLedOn) {
        logger.debug("CMD ONBOARD_LED OFF accepted.");
        addCommand(new TurnOffCommand(&fireAlarmController));
        isOnboardLedOn = false;
    }
}

// LED Strip Control
void ActuatorManager::requestLedStripGreen(size_t level) noexcept {
    if (!isLedStripOn || ledStripcolor != "green" || ledStripLevel != level) {
        logger.debug("CMD LEDSTRIP GREEN level %d accepted.", level);
        addCommand(new SetLedStripPatternCommand(&ledStripController, Color::Green(), level, false));
        isLedStripOn = true;
        ledStripcolor = "green";
        ledStripLevel = level;
    }
}

void ActuatorManager::requestLedStripOrange(size_t level) noexcept {
    if (!isLedStripOn || ledStripcolor != "orange" || ledStripLevel != level) {
        logger.debug("CMD LEDSTRIP ORANGE level %d accepted.", level);
        addCommand(new SetLedStripPatternCommand(&ledStripController, Color::Orange(), level));
        isLedStripOn = true;
        ledStripcolor = "orange";
        ledStripLevel = level;
    }
}

void ActuatorManager::requestLedStripRed(size_t level) noexcept {
    if (!isLedStripOn || ledStripcolor != "red" || ledStripLevel != level) {
        logger.debug("CMD LEDSTRIP RED level %d accepted.", level);
        addCommand(new SetLedStripPatternCommand(&ledStripController, Color::Red(), level));
        isLedStripOn = true;
        ledStripcolor = "red";
        ledStripLevel = level;
    }
}

void ActuatorManager::requestLedStripOff() noexcept {
    if (isLedStripOn) {
        logger.debug("CMD LEDSTRIP OFF accepted.");
        addCommand(new TurnOffCommand(&ledStripController));
        isLedStripOn = false;
    }
}

void ActuatorManager::requestLedStripOn() noexcept {
    if (!isLedStripOn) {
        logger.debug("CMD LEDSTRIP ON accepted.");
        addCommand(new TurnOnCommand(&ledStripController));
        isLedStripOn = true;
    }
}

void ActuatorManager::addCommand(ICommand* command) noexcept { commandQueue.addCommand(command); }

// Process all commands in the queue
void ActuatorManager::processCommands() noexcept {
    logger.debug("PROCESS CMD.");
    commandQueue.processCommands();
}

void ActuatorManager::init() noexcept {
    ledStripController.initialize();
    fireAlarmController.initialize();
    fanController.initialize();
    coolerController.initialize();
    heaterController.initialize();
    logger.info("Actuators Ready.");
}

float ActuatorManager::getFanSpeed() const noexcept { return fanController.getCurrentSpeed(); }
