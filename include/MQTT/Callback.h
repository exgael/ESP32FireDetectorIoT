#pragma once

#include <EasyLife.h>

#include "../ActuatorManager.h"
#include "../ESPConfig.h"

extern void setupCallback(
    EasyMQTT& client,    
    ActuatorManager& actuatorManager,
    Logger& logger
);

/**
 * @brief Returns a lambda callback for the mqtt message handler.
 */
extern MessageHandler mqttCallback(
    ActuatorManager& actuatorManager,
    Logger& logger
) noexcept;