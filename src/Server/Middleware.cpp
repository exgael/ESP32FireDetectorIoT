#include "Server/Middleware.h"

#include <ArduinoJson.h>
#include <algorithm>

/////////////////////////////
//     LOG MIDDLEWARE      //
/////////////////////////////

MiddlewareFunction logHTTPMethod(Logger &logger)
{
    return [&logger](Request &req, Response &res, NextFunction next) {
        auto httpMethodType = req.getMethod() == HTTP_GET ? "GET" : "POST";
        auto url = req.getURL().c_str();

        logger.info("[%s] %s", httpMethodType, url);

        next();
    };
}

/////////////////////////////
//    ERROR MIDDLEWARE     //
/////////////////////////////

MiddlewareFunction errorHandler(Logger &logger)
{
    return [&logger](Request &req, Response &res, NextFunction next) {
        try {
            next();
        } catch (const std::exception &e) {
            auto errorMessage = e.what();
            logger.error("[Internal error] %s", errorMessage);
            res.error(500, String("Internal error: ") + errorMessage);
        }
    };
}

/////////////////////////////
//  VALIDATION MIDDLEWARE  //
/////////////////////////////

/** ROOT */

MiddlewareFunction rootRequestValidationHandler()
{
    return [](Request &req, Response &res, NextFunction next) {
        if (req.getTotalParamsCount() != 0) {
            res.error(
                400, "Unexpected query, body or file parameter(s) found.");
            return;
        }

        // Pass control to the next handler
        next();
    };
}

/** FETCH VALUES */

MiddlewareFunction fetchRequestValidationHandler()
{
    return [](Request &req, Response &res, NextFunction next) {
        std::vector<String> validQueryParams = {
            "temperature", "light", "is_hotspot", "uptime"
        };

        // Check if any unexpected parameters are present
        if ((req.getBodyParamCount() + req.getFileParamCount()) != 0) {
            res.error(400, "Unexpected non-query parameter(s) found.");
            return;
        }

        // Validate query parameters
        if (!checkValidQueryParams(req, validQueryParams)) {
            res.error(
                400,
                "Invalid query parameter name. Expected: temperature, light, is_hotspot, uptime.");
            return;
        }

        // Pass control to the next handler
        next();
    };
}

/** SET VALUES */

MiddlewareFunction setRequestValidationHandler()
{
    return [](Request &req, Response &res, NextFunction next) {
        std::vector<String> validQueryParams = { "onboardLed", "ledStrip", "ledStripColor" };

        // Validate query parameter names
        if (!checkValidQueryParams(req, validQueryParams)) {
            res.error(
                400,
                "Invalid query parameter name. Expected: onboardLed, ledStrip");
            return;
        }

        // Validate parameter values
        for (const String &param : req.getQueryParamNames()) {
            String value = req.getQueryParamValue(param);

            // Check for "on" or "off" values
            if ((param == "onboardLed" || param == "ledStrip") &&
                !(value == "on" || value == "off")) {
                res.error(
                    400,
                    "Invalid value for '" + param +
                        "'. Expected 'on' or 'off'.");
                return;
            }

            // Check for positive integer values
            if ((param == "ledStripColor") &&
                !(value == "red" || value == "green" || value == "orange")) {
                res.error(
                    400,
                    "Invalid value for '" + param +
                        "'. Expected 'red', 'green' or 'orange'.");
                return;
            }
        }

        // Pass control to the next handler
        next();
    };
}

/////////////////////////
//  VALIDATION HELPER  //
/////////////////////////

bool checkRequiredBodyParams(
    const Request &req,
    const std::vector<String> &requiredParams)
{
    // Ensure exact number of required parameters is present
    if (req.getBodyParamCount() != requiredParams.size()) {
        return false;
    }

    // Check for each required parameter's presence
    for (const String &requiredParam : requiredParams) {
        if (!req.hasBodyParam(requiredParam)) {
            return false;
        }
    }

    return true; // All required parameters are present
}

bool checkValidQueryParams(
    const Request &req,
    const std::vector<String> &validQueryParams)
{
    std::vector<String> queryParamNames = req.getQueryParamNames();

    // Check if each query parameter in the request is valid
    for (const String &queryParamName : queryParamNames) {
        if (std::find(
                validQueryParams.begin(),
                validQueryParams.end(),
                queryParamName) == validQueryParams.end()) {
            return false;
        }
    }
    return true;
}
