#include "communication.h"

#include "../storage/s_spiffs.h"  // Make sure the path to s_spiffs.h is correct

extern SSPIFFS spiffs;  // Declare the spiffs object here

Communication::Communication() {}

void Communication::init() {
    WiFi.mode(WIFI_AP);

    if (!WiFi.softAP(spiffs.getWifiAp(), spiffs.getWifiPassword())) {
        Serial.println("Fail Start Wifi");

        return;
    }

    delay(100);

    WiFi.softAPConfig(IPAddress(4, 3, 2, 1), IPAddress(4, 3, 2, 1),
                      IPAddress(255, 255, 255, 0));

    dnsServer.start(53, "*", WiFi.softAPIP());
    Serial.println("Success Start Wifi");
}

void Communication::loop() { dnsServer.processNextRequest(); }
