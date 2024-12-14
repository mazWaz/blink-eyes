#ifndef S_SPIFFS_H
#define S_SPIFFS_H

#include <ArduinoJson.h>
#include <FS.h>
#include <SPIFFS.h>
#include <config.h>

#include "config.h"

class SSPIFFS {
   private:
    bool loadConfig();

   public:
    struct Config {
        char wifiAp[64];
        char wifiPassword[64];
        char setFile[64];
    };
    Config config;
    bool init();
    bool saveWifi(uint8_t wifiAP, uint8_t password);
    bool saveGif(String gifFileName);
    char* getGifFile() { return config.setFile; }
    char* getWifiAp() { return config.wifiAp; }
    char* getWifiPassword() { return config.wifiPassword; }
};

#endif