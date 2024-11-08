/*
 * Author: Benoît Barbier
 */

#pragma once

#include <ESPAsyncWebServer.h>

/**
 * @brief this class wraps `AsyncWebServerRequest`
 */
class Request {
   public:
    explicit Request(AsyncWebServerRequest* raw) : rawRequest(raw) {}

    size_t getParamSize() const { return rawRequest->params(); }

    String arg(String name) { return rawRequest->arg(name.c_str()); }

    bool hasArg(String name) { return rawRequest->hasArg(name.c_str()); }

    AsyncWebParameter* getParam(size_t num) const { return rawRequest->getParam(num); }

    AsyncWebServerRequest* raw() { return rawRequest; }

    bool validated = false;
    String error;

   private:
    AsyncWebServerRequest* rawRequest;
};
