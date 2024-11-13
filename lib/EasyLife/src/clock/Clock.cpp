/*
 * Author: Benoît Barbier
 */

#include "Clock.h"

Clock &Clock::sharedInstance()
{
    static Clock instance;
    return instance;
}

Clock::Clock()
    : startTime(millis()), lastUpdate(millis()), deltaTime(0), elapsedTime(0)
{
}

void Clock::update()
{
    unsigned long currentTime = millis();
    deltaTime = currentTime - lastUpdate; // Time since last update
    elapsedTime = currentTime - startTime; // Total time since start
    lastUpdate = currentTime;
}

unsigned long Clock::getDeltaTime() const
{
    return deltaTime;
}
unsigned long Clock::getElapsedTime() const
{
    return elapsedTime;
}
bool Clock::hasTimePassed(unsigned long &timer, unsigned long interval)
{
    if (elapsedTime - timer >= interval) {
        timer = elapsedTime; // Reset the timer
        return true;
    }
    return false;
}

void Clock::reset()
{
    startTime = millis();
    lastUpdate = startTime;
    elapsedTime = 0;
    deltaTime = 0;
}
