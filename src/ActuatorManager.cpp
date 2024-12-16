/*
 * Author: Benoît Barbier
 */

#include "ActuatorManager.h"

ActuatorManager::ActuatorManager(
    OnboardLedController onboardLedController,
    LedStripController ledStripController)
    : onboardLedController(onboardLedController),
      ledStripController(ledStripController),
      logger("ActuatorManager")
{
}

void ActuatorManager::requestOnboardLedOn() noexcept
{
    if (!isOnboardLedOn) {
        logger.debug("CMD ONBOARD_LED ON accepted.");
        addCommand(new TurnOnCommand(&onboardLedController));
        isOnboardLedOn = true;
    }
}

void ActuatorManager::requestOnboardLedOff() noexcept
{
    if (isOnboardLedOn) {
        logger.debug("CMD ONBOARD_LED OFF accepted.");
        addCommand(new TurnOffCommand(&onboardLedController));
        isOnboardLedOn = false;
    }
}

void ActuatorManager::requestLedStripGreen() noexcept
{
    if (!isLedStripOn || ledStripcolor != "green") {
        logger.debug("CMD LEDSTRIP GREEN.");
        addCommand(new SetLedStripPatternCommand(
            &ledStripController, Color::Green(), ALL_LIGHT));
        isLedStripOn = true;
        ledStripcolor = "green";
    }
}

void ActuatorManager::requestLedStripOrange() noexcept
{
    if (!isLedStripOn || ledStripcolor != "orange") {
        logger.debug("CMD LEDSTRIP ORANGE.");
        addCommand(new SetLedStripPatternCommand(
            &ledStripController, Color::Orange(), ALL_LIGHT));
        isLedStripOn = true;
        ledStripcolor = "orange";
    }
}

void ActuatorManager::requestLedStripRed() noexcept
{
    if (!isLedStripOn || ledStripcolor != "red") {
        logger.debug("CMD LEDSTRIP RED.");
        addCommand(new SetLedStripPatternCommand(
            &ledStripController, Color::Red(), ALL_LIGHT));
        isLedStripOn = true;
        ledStripcolor = "red";
    }
}

void ActuatorManager::requestLedStripOff() noexcept
{
    if (isLedStripOn) {
        logger.debug("CMD LEDSTRIP OFF accepted.");
        addCommand(new TurnOffCommand(&ledStripController));
        isLedStripOn = false;
    }
}

void ActuatorManager::requestLedStripOn() noexcept
{
    if (!isLedStripOn) {
        logger.debug("CMD LEDSTRIP ON accepted.");
        addCommand(new TurnOnCommand(&ledStripController));
        isLedStripOn = true;
    }
}

void ActuatorManager::addCommand(ICommand *command) noexcept
{
    commandQueue.addCommand(command);
}

void ActuatorManager::processCommands() noexcept
{
    logger.debug("PROCESS CMD.");
    commandQueue.processCommands();
}

void ActuatorManager::init() noexcept
{
    onboardLedController.initialize();
    ledStripController.initialize();
    logger.info("Actuators Ready.");
}
