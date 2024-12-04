#pragma once

#include "ESPConfig.h"
#include "FireDetector.h"
#include "SensorManager.h"
#include "TemperatureRegulator.h"
#include "WiFiModule.h"

#include <Easylife.h>
#include <map>

/////////////////////////////
//          ROOT           //
/////////////////////////////

/**
 * @brief Handle returning html.index file.
 */
extern Handler rootControllerHandler(
    ActuatorManager &actuatorManager,
    SensorManager &sensorManager,
    TemperatureRegulator &regulator,
    FireDetector &fireDetector,
    WiFiModule &wifiModule) noexcept;

/////////////////////////////
//          FETCH          //
/////////////////////////////

/**
 * @brief Handles the value fetch.
 */
extern Handler getValuesControllerHandler(
    SensorManager &sensorManager,
    TemperatureRegulator &regulator,
    FireDetector &fireDetector) noexcept;

/////////////////////////////
//           SET           //
/////////////////////////////

/**
 * @brief Handles the value set.
 */
extern Handler setValuesControllerHandler(
    ActuatorManager &actuatorManager,
    SensorManager &sensorManager,
    TemperatureRegulator &regulator,
    FireDetector &fireDetector);