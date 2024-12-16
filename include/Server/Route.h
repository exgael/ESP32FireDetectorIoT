#pragma once

#include "Controller.h"
#include "Middleware.h"

/**
 * @brief Setup the router, middlewares and controller handler.
 */
void setupRouteHandlers(
    EasyServer &server,
    ActuatorManager &actuatorManager,
    SensorManager &sensorManager,
    Hotspot &hotspot,
    WiFiModule &wifiModule,
    Logger &logger) noexcept;