#pragma once

#include <Easylife.h>

// Middleware function
MiddlewareFunction logHTTPMethod(Logger& logger);
MiddlewareFunction errorHandler(Logger& logger);

MiddlewareFunction rootRequestValidationHandler();
MiddlewareFunction fetchRequestValidationHandler();
MiddlewareFunction setRequestValidationHandler();
MiddlewareFunction reportRequestValidationHandler();

// Helper function
bool checkValidQueryParams(const Request& req, const std::vector<String>& validQueryParams);
bool checkRequiredBodyParams(const Request& req, const std::vector<String>& requiredParams);
bool isPositiveOrZeroInteger(const String& value);
bool isIntegerParam(const String& param);