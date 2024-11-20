#include "MQTT/Callback.h"

extern void setupCallback(
    EasyMQTT& client,
    AmIHotspot& hotspot,
    Logger& logger) {
    client.setBufferSize(512);
    client.setMessageHandler(mqttCallback(hotspot, logger));
}

extern MessageHandler mqttCallback(
    AmIHotspot& hotspot,
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

        // Parse JSON payload
        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, message);
        if (error) {
            logger.error("Failed to parse JSON: %s", error.c_str());
            return;
        }

        if (String(topic) == ESPConfig::sharedInstance().TOPIC_TEMP) {
            JsonDocument doc;
            DeserializationError error = deserializeJson(doc, message);

            if (error) {
                logger.error("JSON parse error: %s", error.c_str());
                return;
            }

            // Extract the "piscine" object
            JsonObject piscine = doc["piscine"];
            if (piscine.isNull()) {
                logger.error("Missing 'piscine' object in payload.");
                return;
            }

            // Parse piscine fields
            String ident = doc["info"]["ident"] | "";
            bool hotspotFlag = piscine["hotspot"] | false;
            bool occuped = piscine["occuped"] | false;
            double lat = doc["location"]["gps"]["lat"] | 0.0;
            double lon = doc["location"]["gps"]["lon"] | 0.0;
            double temperature = doc["status"]["temperature"] | 0.0;

            if (ident.isEmpty()) {
                logger.error("Missing 'ident' in payload.");
                return;
            }

            if (lat == 0.0 || lon == 0.0) {
                 logger.error("Missing 'lat/lon' in message %s", message.c_str());
                return;
            }

            // Log the parsed data
            logger.debug("Parsed Piscine: ident=%s, temperature=%f, hotspot=%d, occuped=%d, lat=%f, lon=%f",
                         ident.c_str(), temperature, hotspotFlag, occuped, lat, lon);

            // Add to AmIHotspot
            hotspot.add(ESPPoolStatus(ident, lat, lon, hotspotFlag, occuped, temperature));

            logger.info("Added piscine with ident=%s to hotspot.", ident.c_str());
        }
    };
}