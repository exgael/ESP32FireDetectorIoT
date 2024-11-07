/*
 * Author: Benoît Barbier
 */


#pragma once
#include <Arduino.h>

// https://github.com/WordRivalry/BattleServer/blob/main/src/modules/ecs/systems/timer/TimerSystem.ts
// Singleton
class Clock {
   public:
    /**
     * @brief Get the singleton instance.
     */
    static Clock& sharedInstance();

    /**
     * @brief Update the clock.
     */
    void update();

    unsigned long getDeltaTime() const;
    unsigned long getElapsedTime() const;

    /**
     * @brief Check if a certain amount of time has passed.
     * 
     * Also reset the timer as a side effect if enough time as passed.
     */
    bool hasTimePassed(unsigned long& timer, unsigned long interval);

    /**
     * @brief Reset clock start time.
     */
    void reset();

   private:
    Clock();
    Clock(const Clock&) = delete;
    Clock& operator=(const Clock&) = delete;

    unsigned long startTime;    // Start time of the clock
    unsigned long lastUpdate;   // Last time `update` was called
    unsigned long deltaTime;    // Time elapsed since last frame (delta time)
    unsigned long elapsedTime;  // Total elapsed time since clock started
};
