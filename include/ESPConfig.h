/*
 * Author: Benoît Barbier
 */

#pragma once
#include <ArduinoJson.h>

///////////////////////////
//     ALL DEFAULTS      //
///////////////////////////
class ESPConfig {
   public:
    /**
     * @brief Get the configuration instance.
     */
    static ESPConfig &sharedInstance()
    {
        static ESPConfig instance;
        return instance;
    }

    ///////////////////////////
    //   WIFI CONFIGURATION  //
    ///////////////////////////

    enum class WifiType { BASIC, SEARCH_NEIGHBOR, CONNECT_MULTI };

    // YOU CAN CHANGE HERE :
    WifiType wifiType = WifiType::BASIC;

    // String SSID = "Numericable-ab9e";
    // String PSSWD = "gexlg2s9565t";

    String SSID = "iphone";
    String PSSWD = "Yousra266!";

    ///////////////////////////
    //    SERVER SETTINGS    //
    ///////////////////////////

    int PORT = 80;
    String USER = "Benoit Barbier";

    ///////////////////////////
    //     MQTT SETTINGS     //
    ///////////////////////////

    String MQTT_SERVER = "test.mosquitto.org";
    String TOPIC_TEMP = "uca/iot/piscine";
    String TOPIC_LED = "uca/iot/led";

    ///////////////////////////
    //   DEVICE INFORMATION  //
    ///////////////////////////

    // Identification
    String NAME = "IoT - Device 001";
    String IDENT = "007 ~(^-^)~";
    String ROOM = "3C";
    String ADDRESS = "Les Lucioles";
    String LOC = "Biot";
    double LATITUDE = 43.666672;
    double LONGITUDE = 7.15;

    ///////////////////////////
    //       PIN CONFIG      //
    ///////////////////////////

    int TEMP_SENSOR_PIN = 4;
    int FAN_PIN = 27;
    int HEATER_LED_PIN = 21;
    int AC_LED_PIN = 19;
    int LED_STRIP_PIN = 13;
    int LIGHT_SENSOR_PIN = 33;
    int ONBOARD_LED_PIN = 2;

    // Fan PWM Channel
    int FAN_PWM_CHANNEL = 0;

    ///////////////////////////
    //     FIRE DETECTION    //
    ///////////////////////////

    int DETECTION_BUFFER_SIZE = 5;

    // Fire detection thresholds
    int FIRE_LIGHT_THRESHOLD = 3000;
    int FIRE_TEMPERATURE_THRESHOLD = 26;

    ///////////////////////////
    //       LED STRIP       //
    ///////////////////////////

    size_t LED_STRIP_SIZE = 5;

    ///////////////////////////
    //    REGUL THRESHOLDS   //
    ///////////////////////////

    // Regulation range
    int LOW_TEMP = 20;
    int HIGH_TEMP = 24;

    ///////////////////////////
    //    REGUL THRESHOLDS   //
    ///////////////////////////

    int OCCUPATION_THRESHOLD = 2000;
    double HOTSPOT_DETECTION_RADIUS = 10;

   private:
    ESPConfig() = default;
    ESPConfig(const ESPConfig &) = delete;
    ESPConfig &operator=(const ESPConfig &) = delete;
};
