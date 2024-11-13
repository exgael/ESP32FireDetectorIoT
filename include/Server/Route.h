#pragma once

#include "Controller.h"
#include "Middleware.h"

/**
 * @brief Setup the router, middlewares and controller handler.
 */
void setupRouteHandlers(
    EasyServer& server,
    ActuatorManager& actuatorManager,
    SensorManager& sensorManager,
    TemperatureRegulator& regulator,
    FireDetector& fireDetector,
    WiFiModule& wifiModule,
    Reporter& reporter,
    Logger& logger
) noexcept;