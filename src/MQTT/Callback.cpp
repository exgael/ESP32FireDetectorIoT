#include "MQTT/Callback.h"

extern void setupCallback(EasyMQTT &client, Hotspot &hotspot, Logger &logger)
{
    client.setBufferSize(512);
    client.setMessageHandler(mqttCallback(hotspot, logger));
}

extern MessageHandler mqttCallback(Hotspot &hotspot, Logger &logger) noexcept
{
    // Callback when a message is published on a subscribed topic.
    return [&](char *topic, byte *payload, unsigned int length) {
        logger.debug("Message arrived on topic: %s", topic);

        // Convert the byte payload
        String message;
        for (unsigned int i = 0; i < length; ++i) {
            message += (char)payload[i];
        }

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

            // Parse piscine fields
            String ident = doc["info"]["ident"] | "";
            bool hotspotFlag = false;
            bool occuped = false;
            double lat = 0.0;
            double lon = 0.0;
            double temperature = 0.0;

            // Check ident
            if (ident.isEmpty()) {
                logger.error("Missing 'ident' in payload.");
                return;
            }

            ////////////////////
            //    Piscine     //
            ////////////////////

            // Extract the "piscine" object
            JsonObject piscine = doc["piscine"];
            if (piscine.isNull()) {
                logger.error("Missing 'piscine' object in payload.");
                return;
            }

            // Validate "piscine" object
            if (!doc.containsKey("piscine")) {
                logger.error(
                    "Missing 'piscine' in payload. Ident: %s", ident.c_str());
                return;
            }

            // Extract piscine fields
            hotspotFlag = piscine["hotspot"] | false;
            occuped = piscine["occuped"] | false;

            /////////////////////
            //    Location     //
            /////////////////////

            // Validate location fields
            if (!doc.containsKey("location") ||
                !doc["location"].containsKey("gps")) {
                logger.error(
                    "Missing 'location.gps' in payload. Ident: %s",
                    ident.c_str());
                return;
            }

            lat = doc["location"]["gps"]["lat"] | 0.0;
            lon = doc["location"]["gps"]["lon"] | 0.0;

            // Check location
            if (lat == 0.0 || lon == 0.0) {
                logger.error(
                    "Invalid or missing 'lat/lon' in payload. Ident: %s. "
                    "Message: %s",
                    ident.c_str(),
                    message.c_str());
                return;
            }

            /////////////////////
            //     Status      //
            /////////////////////

            // Validate status fields
            if (!doc.containsKey("status") ||
                !doc["status"].containsKey("temperature")) {
                logger.error(
                    "Missing 'status.temperature' in payload. Ident: %s",
                    ident.c_str());
                return;
            }

            temperature = doc["status"]["temperature"] | 0.0;

            /////////////////////
            //     Parsed      //
            /////////////////////

            logger.debug(
                "Payload parsed successfully. Ident: %s", ident.c_str());

            // Check if the ident matches the ESP's own ident
            if (ident == ESPConfig::sharedInstance().IDENT) {
                logger.debug(
                    "Not adding self to list. Ident: %s", ident.c_str());
                return;
            }

            /////////////////////
            //     Adding      //
            /////////////////////

            // Add to list
            auto result = hotspot.add(ESPPoolStatus(
                ident, lat, lon, hotspotFlag, occuped, temperature));

            if (result == 1) {
                logger.info(
                    "Device status with ident=%s is not hotspot.",
                    ident.c_str());
            } else if (result == 2) {
                logger.info(
                    "Updated device status with ident=%s to hotspot.",
                    ident.c_str());
            }
        }
    };
}