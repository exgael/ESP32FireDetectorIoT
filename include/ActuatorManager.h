/*
 * Author: Benoît Barbier
 */

#pragma once

#include <EasyLife.h>

#define ALL_LIGHT 100

/**
 * @brief This class provides a central API to execute actuating commands.
 */
class ActuatorManager {
   public:
    ActuatorManager(
        OnboardLedController onboardLedController,
        LedStripController ledStripController);

    // Onboard LED Control
    void requestOnboardLedOn() noexcept;
    void requestOnboardLedOff() noexcept;

    // LED Strip Control
    void requestLedStripGreen() noexcept;
    void requestLedStripOrange() noexcept;
    void requestLedStripRed() noexcept;
    void requestLedStripOff() noexcept;
    void requestLedStripOn() noexcept;

    // Initialization
    void init() noexcept;

    // Execute all commands stored
    void processCommands() noexcept;

   private:
    CommandQueue commandQueue;
    Logger logger;

    // Actuators
    LedStripController ledStripController;
    OnboardLedController onboardLedController;

    // Actuator Flags
    bool isOnboardLedOn = false;
    bool isLedStripOn = false;
    String ledStripcolor = "";

    /**
     * @brief Helper method to add a command.
     */
    void addCommand(ICommand *command) noexcept;
};
