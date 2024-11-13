/*
 * Author: Benoît Barbier
 */

#pragma once

#include "ESPConfig.h"

#include <EasyLife.h>
#include <WiFi.h>

/**
 * @brief wrap wifi_utils and add loggings
 */
class WiFiModule {
   public:
    WiFiModule(
        const String &hostname,
        const String &ssid,
        const String &password);

    void init();
    void printStatus() const noexcept;

    String getHostname()
    {
        return hostname;
    }
    String getSSID()
    {
        return ssid;
    }
    String getPassword()
    {
        return password;
    }
    String getMAC()
    {
        return WiFi.macAddress();
    }
    String getIP()
    {
        return WiFi.localIP().toString();
    }

   private:
    String hostname;
    String ssid;
    String password;
    Logger logger;
};
