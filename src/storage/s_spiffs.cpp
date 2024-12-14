#include "s_spiffs.h"

bool SSPIFFS::init() {
    if (!SPIFFS.begin(true)) {
        Serial.println("Fail Begin SPIFFS");

        return false;
    }
    loadConfig();
    Serial.println("Success Begin SPIFFS");

    return true;
}

bool SSPIFFS::loadConfig() {
    if (!SPIFFS.exists("/config.json")) {
        File file = SPIFFS.open("/config.json", FILE_WRITE);

        if (!file) {
            Serial.println("Fail Open SPIFFS");

            return false;
        }

        JsonDocument docConfig;
        docConfig["wifiAp"] = WIFI_NAME;
        docConfig["wifiPassword"] = WIFI_PASSWORD;
        docConfig["setFile"] = "";

        if (serializeJson(docConfig, file) == 0) {
            Serial.println("Load Fail Serialize Json ");
            return false;
        }

        strlcpy(config.wifiAp, docConfig["wifiAp"], sizeof(config.wifiAp));
        strlcpy(config.wifiPassword, docConfig["wifiPassword"],
                sizeof(config.wifiPassword));
        strlcpy(config.setFile, docConfig["setFile"], sizeof(config.setFile));

        file.close();

        Serial.println("Success Load Config");

        return true;
    }

    File file = SPIFFS.open("/config.json", FILE_READ);

    if (!file) {
        file.close();

        Serial.println("Fail Open SPIFFS");

        return false;
    }

    JsonDocument docConfig;
    DeserializationError error = deserializeJson(docConfig, file);

    if (error) {
        return false;
    }

    strlcpy(config.wifiAp, docConfig["wifiAp"], sizeof(config.wifiAp));
    strlcpy(config.wifiPassword, docConfig["wifiPassword"],
            sizeof(config.wifiPassword));
    strlcpy(config.setFile, docConfig["setFile"], sizeof(config.setFile));

    // code here
    file.close();
    return true;
}

bool SSPIFFS::saveWifi(uint8_t wifiAP, uint8_t password) {
    if (!SPIFFS.exists("/config.json")) {
        File file = SPIFFS.open("/config.json", FILE_WRITE);
        if (!file) {
            Serial.println("Fail Open SPIFFS");
            return false;
        }
        JsonDocument docConfig;
        docConfig["wifiAp"] = wifiAP;
        docConfig["wifiPassword"] = password;
        docConfig["setFile"] = config.setFile;
        if (serializeJson(docConfig, file) == 0) {
            return false;
        }
        strlcpy(config.wifiAp, docConfig["wifiAp"], sizeof(config.wifiAp));
        strlcpy(config.wifiPassword, docConfig["wifiPassword"],
                sizeof(config.wifiPassword));

        file.close();
        return true;
    }
    return false;
}

bool SSPIFFS::saveGif(String gifFileName) {
    if (!SPIFFS.exists("/config.json")) {
        File file = SPIFFS.open("/config.json", FILE_WRITE);
        if (!file) {
            return false;
        }

        JsonDocument docConfig;
        docConfig["wifiAp"] = config.wifiAp;
        docConfig["wifiPassword"] = config.wifiPassword;
        docConfig["setFile"] = gifFileName;

        if (serializeJson(docConfig, file) == 0) {
            return false;
        }

        strlcpy(config.setFile, docConfig["setFile"], sizeof(config.setFile));

        file.close();

        return true;
    }
    return false;
}
