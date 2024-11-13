/*
 * Author: Benoît Barbier
 */

#pragma once

#include <ESPAsyncWebServer.h>

/**
 * @brief this class wraps `AsyncWebServerRequest`
 */
class Request {
   public:
    explicit Request(AsyncWebServerRequest* raw) : rawRequest(raw) {}

    ////////////////////////////
    //        METADATA        //
    ////////////////////////////

    WebRequestMethodComposite getMethod() const {
        return rawRequest->method();
    }

    String getURL() const {
        return rawRequest->url();
    }

    ////////////////////////////
    //  GET PARAMETERS NAMES  //
    ////////////////////////////

    /**
     * @return A list of all the Query parameters names available.
     */
    std::vector<String> getQueryParamNames() const {
        std::vector<String> names;
        for (size_t i = 0; i < rawRequest->params(); i++) {
            AsyncWebParameter* param = rawRequest->getParam(i);
            if (!param->isPost() && !param->isFile()) {
                names.push_back(param->name());
            }
        }
        return names;
    }

    /**
     * @return A list of all the body parameters names available.
     */
    std::vector<String> getBodyParamNames() const {
        std::vector<String> names;
        for (size_t i = 0; i < rawRequest->params(); i++) {
            AsyncWebParameter* param = rawRequest->getParam(i);
            // https://github.com/me-no-dev/ESPAsyncWebServer?tab=readme-ov-file#get-post-and-file-parameters
            // Must not be a file
            if (param->isPost() && !param->isFile()) {
                names.push_back(param->name());
            }
        }
        return names;
    }

    /**
     * @return A list of all the file parameters names available.
     */
    std::vector<String> getFileParamNames() const {
        std::vector<String> names;
        for (size_t i = 0; i < rawRequest->params(); i++) {
            AsyncWebParameter* param = rawRequest->getParam(i);
            // https://github.com/me-no-dev/ESPAsyncWebServer?tab=readme-ov-file#get-post-and-file-parameters
            // If is file, post is also true
            if (param->isFile() && param->isPost()) {
                names.push_back(param->name());
            }
        }
        return names;
    }

    ////////////////////////////
    //  GET PARAMETERS VALUE  //
    ////////////////////////////

    /**
     * @brief Get the query parameter value. 
     * @return String: contain the parameter, return "" if no parameter found.
     * @warning Performs `hasQueryParam` to check whether the parameter exist.  
     */
    String getQueryParamValue(const String& name) const {
        if (hasQueryParam(name)) {
            return rawRequest->getParam(name, false, false)->value();
        }
        return "";
    }

    /**
     * @brief Get the body parameter value. 
     * @return String: contain the parameter, return "" if no parameter found.
     * @warning Performs `hasBodyParam` to check whether the parameter exist.  
     */
    String getBodyParamValue(const String& name) const {
        if (hasBodyParam(name)) {
            return rawRequest->getParam(name, true, false)->value();
        }
        return "";
    }


    ////////////////////////////
    //  GET PARAMETERS COUNT  //
    ////////////////////////////

    /**
     * @brief 
     */
    size_t getTotalParamsCount() const { 
        return rawRequest->params(); 
    }

    /**
     * @brief Get how many query parameter are present in the request
     */
    size_t getQueryParamCount() const {
        size_t count = 0;
        for (size_t i = 0; i < rawRequest->params(); ++i) {
            AsyncWebParameter* param = rawRequest->getParam(i);
            if (!param->isPost() && !param->isFile()) {
                ++count;
            }
        }
        return count;
    }

    /**
     * @brief Get how many body parameter are present in the request
     */
    size_t getBodyParamCount() const {
        size_t count = 0;
        for (size_t i = 0; i < rawRequest->params(); ++i) {
            AsyncWebParameter* param = rawRequest->getParam(i);
            if (param->isPost() && !param->isFile()) {
                ++count;
            }
        }
        return count;
    }

    /**
     * @brief Get how many body parameter are present in the request
     */
    size_t getFileParamCount() const {
        size_t count = 0;
        for (size_t i = 0; i < rawRequest->params(); ++i) {
            AsyncWebParameter* param = rawRequest->getParam(i);
            if (param->isPost() && param->isFile()) {
                ++count;
            }
        }
        return count;
    }

    /////////////////////////////
    //  CHECK PARAM EXISTANCE  //
    /////////////////////////////

    /**
     * @brief Check wheter this query parameter exist
     */
    bool hasQueryParam(const String& name) const {
        return rawRequest->hasParam(name, false, false);
    }

    /**
     * @brief Check wheter this body parameter exist
     */
    bool hasBodyParam(const String& name) const {
        return rawRequest->hasParam(name, true, false);
    }

    /**
     * @brief Check wheter this file parameter exist
     */
    bool hasFileParam(const String& name) const {
        return rawRequest->hasParam(name, true, true);
    }

   private:
    AsyncWebServerRequest* rawRequest;

    // HELPERS
    String arg(const String& name) const { return rawRequest->arg(name.c_str()); }
    bool hasArg(const String& name) const { return rawRequest->hasArg(name.c_str()); }
    AsyncWebParameter* getParam(size_t num) const { return rawRequest->getParam(num); }
    AsyncWebServerRequest* raw() { return rawRequest; }
};
