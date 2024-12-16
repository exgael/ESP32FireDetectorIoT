
#include "Server/Route.h"

void setupRouteHandlers(
    EasyServer &server,
    ActuatorManager &actuatorManager,
    SensorManager &sensorManager,
    Hotspot &hotspot,
    WiFiModule &wifiModule,
    Logger &logger) noexcept
{
    server.use(logHTTPMethod(logger));
    server.use(errorHandler(logger));

    server.get(
        "/",
        { // Middlewares
          rootRequestValidationHandler() },
        // Handler
        rootControllerHandler(
            actuatorManager,
            sensorManager,
            hotspot,
            wifiModule));

    server.get(
        "/value",
        { // Middlewares
          fetchRequestValidationHandler() },
        // Handler
        getValuesControllerHandler(sensorManager, hotspot));

    server.get(
        "/set",
        { // Middlewares
          setRequestValidationHandler() },
        // Handler
        setValuesControllerHandler(
            actuatorManager, sensorManager));

    server.onNotFound([&logger](Request &req, Response &res) {
        logger.warn("http request gave 404.");
        res.send(404);
    });
}