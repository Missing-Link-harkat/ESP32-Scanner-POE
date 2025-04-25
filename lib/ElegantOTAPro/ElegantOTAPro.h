/**
_____ _                        _    ___ _____  _    
| ____| | ___  __ _  __ _ _ __ | |_ / _ \_   _|/ \   
|  _| | |/ _ \/ _` |/ _` | '_ \| __| | | || | / _ \  
| |___| |  __/ (_| | (_| | | | | |_| |_| || |/ ___ \ 
|_____|_|\___|\__, |\__,_|_| |_|\__|\___/ |_/_/   \_\
              |___/                                  
*/

/**
 * 
 * @name ElegantOTA Pro
 * @author SOFTT (hello@softt.io)
 * @brief OTA updates made slick and simple for everyone!
 */

#ifndef ElegantOTAPro_h
#define ElegantOTAPro_h

#include "Arduino.h"
#include "stdlib_noniso.h"
#include "elop.h"
#include "logo.h"
#include "esp_ota_ops.h"

#ifndef ELEGANTOTA_USE_ASYNC_WEBSERVER
  #define ELEGANTOTA_USE_ASYNC_WEBSERVER 1
#endif

#ifndef ELEGANTOTA_DEBUG
  #define ELEGANTOTA_DEBUG 1
#endif

#ifndef UPDATE_DEBUG
  #define UPDATE_DEBUG 1
#endif

#if ELEGANTOTA_DEBUG
  #define ELEGANTOTA_DEBUG_MSG(x) Serial.printf("%s %s", "[ElegantOTA] ", x)
#else
  #define ELEGANTOTA_DEBUG_MSG(x)
#endif

#if defined(ESP8266)
  #include <functional>
  #include "FS.h"
  #include "LittleFS.h"
  #include "Updater.h"
  #include "StreamString.h"
  #if ELEGANTOTA_USE_ASYNC_WEBSERVER == 1
    #include "ESPAsyncTCP.h"
    #include "ESPAsyncWebServer.h"
    #define ELEGANTOTA_WEBSERVER AsyncWebServer
  #else
    #include "ESP8266WiFi.h"
    #include "WiFiClient.h"
    #include "ESP8266WebServer.h"
    #define ELEGANTOTA_WEBSERVER ESP8266WebServer
  #endif
  #define HARDWARE "ESP8266"
#elif defined(ESP32)
  #include <functional>
  #include "FS.h"
  #include "Update.h"
  #include "StreamString.h"
  #if ELEGANTOTA_USE_ASYNC_WEBSERVER == 1
    #include "AsyncTCP.h"
    #include "ESPAsyncWebServer.h"
    #define ELEGANTOTA_WEBSERVER AsyncWebServer
  #else
    #include "WiFi.h"
    #include "WiFiClient.h"
    #include "WebServer.h"
    #define ELEGANTOTA_WEBSERVER WebServer
  #endif
  #define HARDWARE "ESP32"
#elif defined(TARGET_RP2040)
  #include <functional>
  #include "Arduino.h"
  #include "FS.h"
  #include "LittleFS.h"
  #include "WiFiClient.h"
  #include "WiFiServer.h"
  #include "WebServer.h"
  #include "WiFiUdp.h"
  #include "StreamString.h"
  #include "Updater.h"
  #define HARDWARE              "RP2040"
  #define ELEGANTOTA_WEBSERVER  WebServer
  // Throw an error if async mode is enabled
  #if ELEGANTOTA_USE_ASYNC_WEBSERVER == 1
    #error "Async mode is not supported on RP2040. Please set ELEGANTOTA_USE_ASYNC_WEBSERVER to 0."
  #endif
  extern uint8_t _FS_start;
  extern uint8_t _FS_end;
#endif

enum OTA_Mode {
    OTA_MODE_FIRMWARE = 0,
    OTA_MODE_FILESYSTEM = 1
};

class ElegantOTAClass{
  public:
    ElegantOTAClass();

    void begin(ELEGANTOTA_WEBSERVER *server, const char * username = "", const char * password = "");

    // Set portal title
    void setTitle(const char * title);

    // Set Hardware ID
    void setID(const char * id);

    // Set Firmware Version
    void setFWVersion(const char * version);

    void setDeveloperMode(bool enable);

    // Authentication
    void setAuth(const char * username, const char * password);
    void clearAuth();

    void setAutoReboot(bool enable);
    void loop();

    void onStart(std::function<void()> callable);
    void onProgress(std::function<void(size_t current, size_t final)> callable);
    void onEnd(std::function<void(bool success)> callable);

    void enableRollback(bool enable);
    void setStabilityCheck(std::function<bool()> check);
    void checkForRollback();
    
    void enableHeartbeatCheck(unsigned long timeout);
    void handleHeartbeatCheck(std::function<void()> callback);
    void updateHeartbeat();
    
  private:
    ELEGANTOTA_WEBSERVER *_server;

    bool _authenticate;
    String _title;
    String _id;
    String _fw_version;
    String _username;
    String _password;
    bool _fw_enabled = true;
    bool _developer_mode = false;

    bool _auto_reboot = true;
    bool _reboot = false;
    unsigned long _reboot_request_millis = 0;

    bool _heartbeat_enabled = false;
    unsigned long _last_heartbeat = 0;
    unsigned long _heartbeat_timeout = 10000;

    String _update_error_str = "";
    unsigned long _current_progress_size;

    std::function<void()> preUpdateCallback = NULL;
    std::function<void(size_t current, size_t final)> progressUpdateCallback = NULL;
    std::function<void(bool success)> postUpdateCallback = NULL;

    bool _rollback_enabled = false;
    std::function<bool()> _stability_check = NULL;
};

extern ElegantOTAClass ElegantOTA;
#endif
