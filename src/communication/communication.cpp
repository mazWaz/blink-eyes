#include "communication.h"

Communication::Communication() {}

void Communication::init() {
    WiFi.mode(WIFI_AP);

    if (!WiFi.softAP(WIFI_NAME, WIFI_PASSWORD)) {
        return;
    }
    delay(100);
    WiFi.softAPConfig(IPAddress(4, 3, 2, 1), IPAddress(4, 3, 2, 1),
                      IPAddress(255, 255, 255, 0));

    dnsServer.start(53, "*", WiFi.softAPIP());
}

void Communication::loop() { dnsServer.processNextRequest(); }
