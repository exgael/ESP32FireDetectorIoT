#include "MQTT/Callback.h"

extern void setupCallback(
    EasyMQTT& client,
    ActuatorManager& actuatorManager,
    Logger& logger) {
    client.setBufferSize(512);
    client.setMessageHandler(mqttCallback(actuatorManager, logger));
}

extern MessageHandler mqttCallback(
    ActuatorManager& actuatorManager,
    Logger& logger
) noexcept {
     // Callback when a message is published on a subscribed topic.
    return [&](char *topic, byte *payload, unsigned int length) {
       
        logger.debug("Message arrived on topic: %s", topic);

        // Convert the byte payload
        String message;
        for (unsigned int i = 0; i < length; ++i) {
            message += (char)payload[i];
        }

        logger.debug("=> %s", message.c_str());

        // If a message is received on the LED topic, change the LED state accordingly
        if (String(topic) == ESPConfig::sharedInstance().TOPIC_LED) {

            auto logMsg = "so ... changing output to %s";

            if (message == "on") {
                logger.debug(logMsg, "on");
                actuatorManager.requestOnboardLedOn();
            } else if (message == "off") {
                logger.debug(logMsg, "off");
                actuatorManager.requestOnboardLedOff();
            }
        }
    };
}