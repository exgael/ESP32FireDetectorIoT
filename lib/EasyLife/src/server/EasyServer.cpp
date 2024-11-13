/*
 * Author: Benoît Barbier
 */

#include "EasyServer.h"

EasyServer::EasyServer(int port)
    : server(port),
      port(port),
      logger("Server") {}

void EasyServer::init() noexcept {
    server.begin();
    logger.info("Server is running on port: %d", this->port);
}

void EasyServer::use(MiddlewareFunction middleware) { globalMiddleware.push_back(middleware); }

/**
 * @brief Adds a `get`route.
 */
void EasyServer::get(const String& path, const MiddlewareChain& middleware, Handler handler) {
    addRoute(path, HTTP_GET, middleware, handler);
}

/**
 * @brief Adds a `post` route.
 */
void EasyServer::post(const String& path, const MiddlewareChain& middleware, Handler handler) {
    addRoute(path, HTTP_POST, middleware, handler);
}

/**
 * @brief On not found.
 */
void EasyServer::onNotFound(Handler handler) {
    server.onNotFound([this, handler](AsyncWebServerRequest* rawRequest) {
        Request req(rawRequest);
        Response res(rawRequest);
        handler(req, res);
    });
}

void EasyServer::addRoute(const String& path, WebRequestMethod method,
                  const MiddlewareChain& routeMiddleware, Handler handler) {
    server.on(path.c_str(), method, [this, routeMiddleware, handler](AsyncWebServerRequest* rawRequest) {
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

MiddlewareChain EasyServer::buildRouteMiddlewareChain(const MiddlewareChain& routeMiddleware) {
    MiddlewareChain allMiddleware;

    // Add globals
    allMiddleware.insert(allMiddleware.end(), globalMiddleware.begin(), globalMiddleware.end());

    // Add route specific
    allMiddleware.insert(allMiddleware.end(), routeMiddleware.begin(), routeMiddleware.end());

    return allMiddleware;
}