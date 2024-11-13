/*
 * Author: Benoît Barbier
 */

#pragma once

#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

/**
 * @brief this class handle the response of the request
 */
class Response {
   public:
    explicit Response(AsyncWebServerRequest *raw) : rawRequest(raw)
    {
    }

    void send(const int statusCode)
    {
        rawRequest->send(statusCode);
    }

    void sendFile(
        String filePath,
        std::function<String(const String &var)> processor)
    {
        rawRequest->send(SPIFFS, filePath, String(), false, processor);
    }

    void sendText(const int statusCode, const String &text)
    {
        rawRequest->send(statusCode, "text/plain", text);
    }

    void sendHtml(const int statusCode, const String &html)
    {
        rawRequest->send(statusCode, "text/html", html);
    }

    void sendJson(const int statusCode, const JsonDocument &json)
    {
        String response;
        serializeJson(json, response);
        rawRequest->send(statusCode, "application/json", response);
    }

    void error(const int statusCode, const String &message)
    {
        if (statusCode >= 200 && statusCode < 300) {
            throw std::runtime_error("No error status code set.");
        }
        rawRequest->send(
            statusCode, "application/json", "{\"error\": \"" + message + "\"}");
    }

    /**
     * @brief access the `AsyncWebServerRequest` object
     * @return return the underlying request object
     * @warning Futur me, if you use this, add a method instead.
     */
    AsyncWebServerRequest *raw()
    {
        return rawRequest;
    }

   private:
    AsyncWebServerRequest *rawRequest;
};
