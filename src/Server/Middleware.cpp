#include <ArduinoJson.h>
#include <algorithm>

#include "Server/Middleware.h"

/////////////////////////////
//     LOG MIDDLEWARE      //
/////////////////////////////

MiddlewareFunction logHTTPMethod(Logger& logger) {
    return [&logger](Request& req, Response& res, NextFunction next) {
        auto httpMethodType = req.getMethod() == HTTP_GET ? "GET" : "POST";
        auto url = req.getURL().c_str();

        logger.info("[%s] %s", httpMethodType, url);

        next();
    };
}

/////////////////////////////
//    ERROR MIDDLEWARE     //
/////////////////////////////

MiddlewareFunction errorHandler(Logger& logger) {
    return [&logger](Request& req, Response& res, NextFunction next) {
        try {
            next();
        } catch (const std::exception& e) {
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

MiddlewareFunction rootRequestValidationHandler() {
    return [](Request& req, Response& res, NextFunction next) {
        if (req.getTotalParamsCount() != 0) {
            res.error(400, "Unexpected query, body or file parameter(s) found.");
            return;
        }

        // Pass control to the next handler
        next();
    };
}

/** FETCH VALUES */

MiddlewareFunction fetchRequestValidationHandler() {
    return [](Request& req, Response& res, NextFunction next) {
        std::vector<String> validQueryParams = {
            "temperature", 
            "light", 
            "running", 
            "fire",
            "uptime",
            "cooler", 
            "FTT", 
            "FLT",
            "LT", 
            "HT"
        };

        // Check if any unexpected parameters are present
        if ((req.getBodyParamCount() + req.getFileParamCount()) != 0) {
            res.error(400, "Unexpected non-query parameter(s) found.");
            return;
        }
        
        // Validate query parameters
        if (!checkValidQueryParams(req, validQueryParams)) {
            res.error(400, "Invalid query parameter name. Expected: cool, heat, lt, ht, FTT, FLT.");
            return;
        }

        // Pass control to the next handler
        next();
    };
}

/** SET VALUES */

MiddlewareFunction setRequestValidationHandler() {
    return [](Request& req, Response& res, NextFunction next) {
        std::vector<String> validQueryParams = {"cool", "heat", "lt", "ht", "FTT", "FLT"};
        
        // Validate query parameter names
        if (!checkValidQueryParams(req, validQueryParams)) {
            res.error(400, "Invalid query parameter name. Expected: cool, heat, lt, ht, FTT, FLT.");
            return;
        }

        // Validate parameter values
        for (const String& param : req.getQueryParamNames()) {
            String value = req.getQueryParamValue(param);

            // Check for "on" or "off" values
            if ((param == "cool" || param == "heat") && !(value == "on" || value == "off")) {
                res.error(400, "Invalid value for '" + param + "'. Expected 'on' or 'off'.");
                return;
            }

            // Check for positive integer values
            if (isIntegerParam(param) && !isPositiveOrZeroInteger(value)) {
                res.error(400, "Invalid value for '" + param + "'. Expected a positive integer.");
                return;
            }
        }

        // Pass control to the next handler
        next();
    };
}

/** SET REPORT */

MiddlewareFunction reportRequestValidationHandler() {
    return [](Request& req, Response& res, NextFunction next) {
        std::vector<String> requiredBodyParams = {"ip", "port", "sp"};

        // Validate query parameter names
        if (!checkRequiredBodyParams(req, requiredBodyParams)) {
            res.error(400, "Invalid query parameter name. Expected: ip, port and sp.");
            return;
        }

        // Validate parameter values
        for (const String& param : requiredBodyParams) {
            String value = req.getBodyParamValue(param);

            // MARK: TODO - Check IP regex
            
            // Check for positive integer values
            if (isIntegerParam(param) && !isPositiveOrZeroInteger(value)) {
                res.error(400, "Invalid value for '" + param + "'. Expected a positive integer.");
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

bool checkRequiredBodyParams(const Request& req, const std::vector<String>& requiredParams) {
    // Ensure exact number of required parameters is present
    if (req.getBodyParamCount() != requiredParams.size()) {
        return false;
    }

    // Check for each required parameter's presence
    for (const String& requiredParam : requiredParams) {
        if (!req.hasBodyParam(requiredParam)) {
            return false;
        }
    }

    return true;  // All required parameters are present
}

bool checkValidQueryParams(const Request& req, const std::vector<String>& validQueryParams) {
    std::vector<String> queryParamNames = req.getQueryParamNames();
    
    // Check if each query parameter in the request is valid
    for (const String& queryParamName : queryParamNames) {
        if (std::find(validQueryParams.begin(), validQueryParams.end(), queryParamName) == validQueryParams.end()) {
            return false;
        }
    }
    return true;
}

bool isPositiveOrZeroInteger(const String& value) {
    int intValue = value.toInt();
    return (intValue >= 0);
}

bool isIntegerParam(const String& param) {
    return (param == "lt" || param == "ht" || param == "FTT" || param == "FLT" || param == "port" || param == "sp");
}