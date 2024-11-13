/*
 * Author: Benoît Barbier
 */

#pragma once
#include "Request.h"
#include "Response.h"

#include <functional>

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
using MiddlewareFunction =
    std::function<void(Request &, Response &, NextFunction)>;

/**
 * @brief Represent a constructed pipeline of middleware to process
 */
using MiddlewareChain = std::vector<MiddlewareFunction>;

/**
 * @brief Represent a controller class
 */
using Handler = std::function<void(Request &, Response &)>;