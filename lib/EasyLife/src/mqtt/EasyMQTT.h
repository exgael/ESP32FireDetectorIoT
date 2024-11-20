#pragma once

#include "../logger/index.h"

#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <WiFi.h>

using MessageHandler =
    std::function<void(char *topic, byte *payload, unsigned int length)>;

class EasyMQTT {
   public:
    EasyMQTT(String mqttclientId, String mqttServer);

    void init();
    void subscribe(String &topic, uint8_t qos);
    void setMessageHandler(MessageHandler msgHandler);
    void publish(String &topic, JsonDocument &payload);
    void publish(String &topic, String &payload);
    void setBufferSize(uint16_t size);
    void loop();

   private:
    PubSubClient mqttClient;
    String mqttServerURL;
    String mqttclientId;
    Logger logger;

    bool isConnected();
    bool connect(String &id);
};