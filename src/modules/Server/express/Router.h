/*
 * Author: Benoît Barbier
 */

#pragma once

#include "Middleware.h"

/**
 * @brief Wraps the action of adding a routes and adds middlesware to the
 * process.
 */
class Router {
   public:
    /**
     * @brief Add global middleware which is executed each time.
     */
    void use(MiddlewareFunction middleware) { globalMiddleware.push_back(middleware); }

    /**
     * @brief Adds a `get`route.
     */
    void get(const String& path, AsyncWebServer& Server, const MiddlewareChain& middleware, Handler handler) {
        addRoute(path, HTTP_GET, Server, middleware, handler);
    }

    /**
     * @brief Adds a `post` route.
     */
    void post(const String& path, AsyncWebServer& Server, const MiddlewareChain& middleware, Handler handler) {
        addRoute(path, HTTP_POST, Server, middleware, handler);
    }

   private:
    MiddlewareChain globalMiddleware;

    /**
     * @brief Add route with specific middleware
     * The best part ~(*-*)~
     */
    void addRoute(const String& path, WebRequestMethod method, AsyncWebServer& Server,
                  const MiddlewareChain& routeMiddleware, Handler handler) {
        Server.on(path.c_str(), method, [this, routeMiddleware, handler](AsyncWebServerRequest* rawRequest) {
            Request req(rawRequest);
            Response res(rawRequest);

            // Combine global and route middleware
            MiddlewareChain allMiddleware = buildRouteMiddlewareChain(routeMiddleware);

            size_t currentIndex = 0;

            // Execute each middleware
            NextFunction next = [&]() {
                if (currentIndex < allMiddleware.size()) {
                    MiddlewareFunction currentMiddleware = allMiddleware[currentIndex];
                    currentIndex++;
                    currentMiddleware(req, res, next);
                } else {
                    handler(req, res);
                }
            };

            next();
        });
    }

    MiddlewareChain buildRouteMiddlewareChain(const MiddlewareChain& routeMiddleware) {
        MiddlewareChain allMiddleware;

        // Add globals
        allMiddleware.insert(allMiddleware.end(), globalMiddleware.begin(), globalMiddleware.end());

        // Add route specific
        allMiddleware.insert(allMiddleware.end(), routeMiddleware.begin(), routeMiddleware.end());

        return allMiddleware;
    }
};
