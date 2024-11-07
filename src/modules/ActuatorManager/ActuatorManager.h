/*
 * Author: Benoît Barbier
 * Created: 2024-11-07
 * Last Modified: 2024-11-07
 */


#pragma once

#include "../../library/index.h"

class ActuatorManager {
   public:
    ActuatorManager(FanController fanController, LedController coolerController, LedController heaterController,
                    OnboardLedController fireAlarmController, LedStripController ledStripController);

    // Fan
    void requestFanOn() noexcept;
    void requestFanOff() noexcept;
    void requestFanSpeed(float levle) noexcept;

    // Cooler
    void requestCoolerOn() noexcept;
    void requestCoolerOff() noexcept;

    // Heater
    void requestHeaterOn() noexcept;
    void requestHeaterOff() noexcept;

    // On board
    void requestOnboardLedOn() noexcept;
    void requestOnboardLedOff() noexcept;

    // Led strip
    void requestLedStripGreen(size_t level) noexcept;
    void requestLedStripOrange(size_t level) noexcept;
    void requestLedStripRed(size_t level) noexcept;
    void requestLedStripOff() noexcept;
    void requestLedStripOn() noexcept;

    // init
    void init() noexcept;

    //  Getter
    [[nodiscard]] float getFanSpeed() const noexcept;

    // Execute all cmds stored
    void processCommands() noexcept;

   private:
    CommandQueue commandQueue;
    Logger logger;

    // Actuators
    LedStripController ledStripController;
    OnboardLedController fireAlarmController;
    FanController fanController;
    LedController coolerController;
    LedController heaterController;

    // Actuator Flags
    bool isFanOn = false;
    float fanSpeed = 0.0;

    bool isCoolerOn = false;
    bool isHeaterOn = false;
    bool isOnboardLedOn = false;

    bool isLedStripOn = false;
    size_t ledStripLevel = 0;
    String ledStripcolor = "";

    /**
     * Helper method to add a command.
     */
    void addCommand(ICommand* command) noexcept;
};
