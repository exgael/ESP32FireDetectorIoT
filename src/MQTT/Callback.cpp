#include "MQTT/Callback.h"

extern void setupCallback(
    EasyMQTT& client,
    ActuatorManager& actuatorManager,
    Logger& logger) {
    client.setBufferSize(512);
    //client.subscribe(ESPConfig::sharedInstance().TOPIC_LED, 2);
    client.setMessageHandler(mqttCallback(actuatorManager, logger));
}

extern MessageHandler mqttCallback(
    ActuatorManager& actuatorManager,
    Logger& logger
) noexcept {
    return [&](char *topic, byte *payload, unsigned int length) {
        // Callback when a message is published on a subscribed topic.
        logger.info("Message arrived on topic: %s", topic);

        // Convert the byte payload to a String and print to Serial
        String message;
        for (unsigned int i = 0; i < length; i++) {
            message += (char)payload[i];
        }

        logger.info("=> %s", message.c_str());

        // If a message is received on the LED topic, change the LED state accordingly
        if (String(topic) == ESPConfig::sharedInstance().TOPIC_LED) {

            auto logMsg = "so ... changing output to %s";

            if (message == "on") {
                logger.info(logMsg, "on");
                actuatorManager.requestOnboardLedOn();
            } else if (message == "off") {
                logger.info(logMsg, "off");
                actuatorManager.requestOnboardLedOff();
            }
        }
    };
}