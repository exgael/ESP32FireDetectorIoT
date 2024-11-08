/*
 * Author: Benoît Barbier
 */

#pragma once
#include <functional>

#include "../../../library/index.h"
#include "Request.h"
#include "Response.h"

/**
 * @brief Used to signal to move to the next handler
 */
using NextFunction = std::function<void()>;

/**
 * @brief A middleware function
 *
 * A middleware function has one job.alignas
 * Provide the context of the Request/Response, allow to move to the next
 * handler.
 */
using MiddlewareFunction = std::function<void(Request&, Response&, NextFunction)>;

/**
 * @brief Represent a constructed pipeline of middleware to process
 */
using MiddlewareChain = std::vector<MiddlewareFunction>;

/**
 * @brief Represent a controller class
 */
using Handler = std::function<void(Request&, Response&)>;

/**
 * @brief Entirely inspired by express framework
 *
 * It works by leveraging a pipeline of function calls.
 *
 * Pipeline:
 *
 * - 1. Global middleware which are exectued each time.
 *
 * - 2. Route specific middlewares which are executed only when the route is
 * used.
 *
 * @warning It is executed in the `ORDER` added!
 */
namespace Middleware {
// Logger middleware
inline MiddlewareFunction logHTTPMethod(Logger& Logger) {
    return [&Logger](Request& req, Response& res, NextFunction next) {
        auto httpMethodType = req.raw()->method() == HTTP_GET ? "GET" : "POST";
        auto url = req.raw()->url().c_str();

        Logger.info("[%s] %s", httpMethodType, url);

        // Call the next handler
        next();
    };
}

// Error handler middleware
inline MiddlewareFunction errorHandler(Logger& Logger) {
    return [&Logger](Request& req, Response& res, NextFunction next) {
        try {
            next();
        } catch (const std::exception& e) {
            auto errorMessage = e.what();
            Logger.error("[Internal error] %s", errorMessage);
            res.status(500);
            res.error(String("Internal error: ") + errorMessage);
        }
    };
}
}  // namespace Middleware
