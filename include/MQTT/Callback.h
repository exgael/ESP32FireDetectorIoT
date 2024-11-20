#pragma once

#include <EasyLife.h>

#include "../ActuatorManager.h"
#include "../ESPConfig.h"
#include "../HotSpot.h"

extern void setupCallback(
    EasyMQTT& client,    
    AmIHotspot& hotspot,
    Logger& logger
);

/**
 * @brief Returns a lambda callback for the mqtt message handler.
 */
extern MessageHandler mqttCallback(
    AmIHotspot& hotspot,

    Logger& logger
) noexcept;