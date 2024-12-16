#pragma once

#include <Easylife.h>

// Middleware function
MiddlewareFunction logHTTPMethod(Logger &logger);
MiddlewareFunction errorHandler(Logger &logger);

MiddlewareFunction rootRequestValidationHandler();
MiddlewareFunction fetchRequestValidationHandler();
MiddlewareFunction setRequestValidationHandler();

// Helper function
bool checkValidQueryParams(
    const Request &req,
    const std::vector<String> &validQueryParams);
bool checkRequiredBodyParams(
    const Request &req,
    const std::vector<String> &requiredParams);