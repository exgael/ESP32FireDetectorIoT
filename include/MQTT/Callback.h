#pragma once

#include "../ActuatorManager.h"
#include "../ESPConfig.h"
#include "../Hotspot.h"

#include <EasyLife.h>

extern void setupCallback(
    EasyMQTT &client,
    Hotspot &hotspot,
    Logger &logger);

/**
 * @brief Returns a lambda callback for the mqtt message handler.
 */
extern MessageHandler mqttCallback(
    Hotspot &hotspot,

    Logger &logger) noexcept;