/*
 * Author: Benoît Barbier
 */


#pragma once

#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"

/**
 * @brief this class handle the response of the request
 */
class Response {
   public:
    explicit Response(AsyncWebServerRequest* raw) : rawRequest(raw) {}

    void status(int code) { statusCode = code; }

    void send200() { rawRequest->send(statusCode); }

    void sendFile(String filePath, std::function<String (const String& var)> processor) {
        rawRequest->send(SPIFFS, filePath, String(), false, processor);
    }

    void sendText(const String& text) { rawRequest->send(statusCode, "text/plain", text); }

    void sendHtml(const String& html) { rawRequest->send(statusCode, "text/html", html); }

    void sendJson(const String& data) { rawRequest->send(statusCode, "application/json", data); }

    void error(const String& message) {
        rawRequest->send(statusCode, "application/json", "{\"error\": \"" + message + "\"}");
    }

    /**
     * @brief access the `AsyncWebServerRequest` object
     * @return return the underlying request object
     * @warning Futur me, if you use this, add a method instead.
     */
    AsyncWebServerRequest* raw() { return rawRequest; }

   private:
    AsyncWebServerRequest* rawRequest;

    int statusCode = 200;
};
