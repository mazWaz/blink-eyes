#include <Arduino.h>
#include <communication/communication.h>
#include <display/display.h>
#include <storage/s_fs.h>
#include <storage/s_spiffs.h>
#include <webService/webservice.h>

SSPIFFS spiffs;
Communication com;
SFS sd;
WebService web(sd);
Display display;

void setup() {
    Serial.begin(115200);
    spiffs.init();
    com.init();
    sd.init();
    web.init();
    delay(2000);
    display.init();
}

void loop() {
    display.loop();
    com.loop();
    web.loop();

    delay(5);
}