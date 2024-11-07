/*
 * Author: Benoît Barbier
 */


#pragma once

#include "../../library/index.h"

#include "../ActuatorManager/index.h"
#include "../SensorManager/index.h"

/**
 * @brief This module handles the temperature regulation, actionning actuators
 * based on readings and fire detection.
 */
class TemperatureRegulator {
   public:
    enum class State { HALT, COOLING, HEATING, EMERGENCY_HALT };

    TemperatureRegulator(ActuatorManager& actuatorManager, float lowerBound, float upperBound);

    ///////////////////////
    //    REGULATION     //
    ///////////////////////

    void regulate(const SensorDataProvider& data, const bool isFireDetected);

    ///////////////////////
    //      STATES       //
    ///////////////////////

    String describeState() const;
    bool isCooling() const noexcept;
    bool isHeating() const noexcept;

    ///////////////////////
    //    THRESHOLDS     //
    ///////////////////////

    int getLowThreshold() const noexcept;
    int getHighThreshold() const noexcept;
    void setLowThreshold(const float newThreshold) noexcept;
    void setHighThreshold(const float newThreshold) noexcept;

   private:
    ///////////////////////
    // CONTROL ACTUATORS //
    ///////////////////////

    void cool(const float temperature);
    void heat(const float temperature);
    void halt(const float temperature);
    void emergencyHalt();

    ///////////////////////
    //     VARIABLES     //
    ///////////////////////

    State current_state;

    float lowerBound;
    float upperBound;

    ActuatorManager& actuatorManager;
    Logger logger;
};
