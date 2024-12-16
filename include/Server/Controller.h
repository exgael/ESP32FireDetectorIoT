#pragma once

#include "ESPConfig.h"
#include "ActuatorManager.h"
#include "SensorManager.h"
#include "Hotspot.h"
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
    Hotspot &hotspot,
    WiFiModule &wifiModule) noexcept;

/////////////////////////////
//          FETCH          //
/////////////////////////////

/**
 * @brief Handles the value fetch.
 */
extern Handler getValuesControllerHandler(
    SensorManager &sensorManager,
    Hotspot &hotspot) noexcept;

/////////////////////////////
//           SET           //
/////////////////////////////

/**
 * @brief Handles the value set.
 */
extern Handler setValuesControllerHandler(
    ActuatorManager &actuatorManager,
    SensorManager &sensorManager);