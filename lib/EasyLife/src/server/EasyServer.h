/*
 * Author: Benoît Barbier
 */

#pragma once

#include <ESPAsyncWebServer.h>

#include "../logger/index.h"
#include "Middleware.h"
#include "Request.h"
#include "Response.h"

class EasyServer {
   public:
    EasyServer(int port);
 
    void init() noexcept;

    /**
     * @brief Add global middleware which is executed each time.
     */
    void use(MiddlewareFunction middleware);

    /**
     * @brief Adds a `get`route.
     */
    void get(const String& path, const MiddlewareChain& middleware, Handler handler);

    /**
     * @brief Adds a `post` route.
     */
    void post(const String& path, const MiddlewareChain& middleware, Handler handler);

    /**
     * @brief On not found.
     */
    void onNotFound(Handler handler);

   private:
    const int port;
    AsyncWebServer server;
    Logger logger;

     MiddlewareChain globalMiddleware;

    /**
     * @brief Add route with specific middleware
     * The best part ~(*-*)~
     */
    void addRoute(const String& path, WebRequestMethod method,
                  const MiddlewareChain& routeMiddleware, Handler handler);
    MiddlewareChain buildRouteMiddlewareChain(const MiddlewareChain& routeMiddleware);
};
