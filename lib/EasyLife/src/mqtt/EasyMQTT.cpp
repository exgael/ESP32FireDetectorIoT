#include "EasyMQTT.h"
#include "../inttypes.h"

EasyMQTT::EasyMQTT(String mqttclientId, String mqttServerURL): 
    mqttclientId(mqttclientId),
    mqttServerURL(mqttServerURL),
    mqttClient(),
    logger("EasyMQTT")
{
}

void EasyMQTT::init() {
    WiFiClient *wifiClient = new WiFiClient();
    mqttClient.setClient(*wifiClient);
    mqttClient.setServer(mqttServerURL.c_str(), 1883);
    logger.info("Mqtt is running on port: %d", 1883);
}

bool EasyMQTT::isConnected() {
    return mqttClient.connected();
}

void EasyMQTT::subscribe(String& topic, uint8_t qos) {
    while(!isConnected()) {
        logger.info("Attempting MQTT connection...");
        if (connect(mqttclientId)) {
            logger.info("MQTT connected as %s.", mqttclientId.c_str());

            if(mqttClient.subscribe(topic.c_str(), qos)) {
                logger.info("MQTT Subscribed to topic: %s", topic.c_str());
            } else {
                logger.warn("MQTT DID NOT Subscribed to topic: %s", topic.c_str());
            }
        } else {
            logger.warn("Connection to broker failed.");
        }
    }
}

void EasyMQTT::publish(String& topic, JsonDocument& payload) {
    String str;
    serializeJson(payload, str);

    if (!isConnected()) {
        logger.error("Is not connected. MQTT DID NOT published to %s", topic.c_str());
        return;
    }

    if(mqttClient.publish(topic.c_str(), str.c_str())) {
        logger.debug("MQTT published to %s", topic.c_str());
    } else {
        logger.error("MQTT DID NOT published to %s", topic.c_str());
    }
}

void EasyMQTT::publish(String& topic, String& payload) {
    
    if (!isConnected()) {
        logger.error("Is not connected. MQTT DID NOT published to %s", topic.c_str());
        return;
    }

    if (mqttClient.publish(topic.c_str(), payload.c_str())) {
        logger.info("MQTT published to %s", topic.c_str());
    } else {
        logger.error("MQTT DID NOT published to %s", topic.c_str());
    }
}

void EasyMQTT::setBufferSize(uint16_t size) {
    // MARK: exploring this... PRIu16
    logger.debug("new buffer size set at: %" PRIu16, size);
    mqttClient.setBufferSize(size);
}

void EasyMQTT::setMessageHandler(MessageHandler msgHandler) {
    logger.debug("New callback set.");
     mqttClient.setCallback(msgHandler);
}

bool EasyMQTT::connect(String& id) {
    return mqttClient.connect(id.c_str(), NULL, NULL);
}

void EasyMQTT::loop() {
    mqttClient.loop();
}

