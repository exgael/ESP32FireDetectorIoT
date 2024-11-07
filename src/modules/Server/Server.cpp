/*
 * Author: Benoît Barbier
 * Created: 2024-11-07
 * Last Modified: 2024-11-07
 */


#include "Server.h"

ESPServer::ESPServer(AsyncWebServer& server, const SensorDataProvider& sensorData, FireDetection& fireDetector,
                     TemperatureRegulator& regulator, ActuatorManager& actuatorManager, Reporter& reporter,
                     WiFiModule& wifiModule)
    : server(server),
      router(),
      controllers(sensorData, fireDetector, regulator, actuatorManager, reporter, wifiModule),
      logger("Server") {}

void ESPServer::init() noexcept {
    setupGlobalMiddlewares();
    setupRoutes();
    server.begin();
}

void ESPServer::setupGlobalMiddlewares() noexcept {
    // Setup middlewares
    router.use(Middleware::logHTTPMethod(this->logger));
    router.use(Middleware::errorHandler(this->logger));
}

void ESPServer::setupRoutes() noexcept {
    router.get("/", server, {},  // No middleware
               [this](Request& req, Response& res) { this->controllers.rootController(req, res); });

    router.get("/value", server, {},  // TODO
               [this](Request& req, Response& res) { this->controllers.getValuesController(req, res); });

    router.get("/set", server, {},  // TODO
               [this](Request& req, Response& res) { this->controllers.setValuesController(req, res); });

    router.post("/target", server, {},  // TODO
                [this](Request& req, Response& res) { this->controllers.setReportController(req, res); });

    server.onNotFound([this](AsyncWebServerRequest* request) {
        this->logger.warn("http request gave 404.");
        request->send(404);
    });
}

void ESPServer::periodicRun() { controllers.sendPeriodicReport(); }
