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

    String getHostname() const noexcept
    {
        return hostname; 
    }
    String getSSID() const
    {
        return ssid;
    }
    String getPassword() const noexcept
    {
        return password;
    } 
    String getMAC() const noexcept
    {
        return WiFi.macAddress();
    }
    String getIP() const noexcept
    {
        return WiFi.localIP().toString();
    } 

   private:
    String hostname;
    String ssid;
    String password;
    Logger logger;
};
