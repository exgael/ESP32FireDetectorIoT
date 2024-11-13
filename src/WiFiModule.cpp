/*
 * Author: Benoît Barbier
 */

#include "WiFiModule.h"

WiFiModule::WiFiModule(
    const String &hostname,
    const String &ssid,
    const String &password)
    : hostname(hostname), ssid(ssid), password(password), logger("WiFiModule")
{
}

void WiFiModule::init()
{
    auto wifiType = ESPConfig::sharedInstance().wifiType;

    switch (wifiType) {
    case ESPConfig::WifiType::BASIC:
        wifi_connect_basic(hostname, ssid, password);
        break;

    case ESPConfig::WifiType::SEARCH_NEIGHBOR:
        if (wifi_search_neighbor()) {
            logger.error("No available wifi spot.");
        }
        break;

    case ESPConfig::WifiType::CONNECT_MULTI:
        wifi_connect_multi(hostname);
        break;

    default:
        logger.error("Unknown WiFi connection type.");
        break;
    }
}

void WiFiModule::printStatus() const noexcept
{
    logger.info("	--- WIFI STATUS ---");
    logger.info("	IP address : %s", WiFi.localIP().toString().c_str());
    logger.info("	MAC address : %s", WiFi.macAddress().c_str());
    logger.info("	SSID : %s", WiFi.SSID().c_str());
    logger.info(
        "	Received Sig Strength Indication :  %sdBm",
        String(WiFi.RSSI()).c_str());
    logger.info(
        "	Received Sig Strength Indication : %s",
        String(constrain(2 * (WiFi.RSSI() + 100), 0, 100)).c_str());
    logger.info("	BSSID : %s", WiFi.BSSIDstr().c_str());
    logger.info(
        "	Encryption type : ",
        translateEncryptionType(WiFi.encryptionType(0)));
    logger.info("");
}
