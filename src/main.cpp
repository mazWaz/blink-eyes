#include <Arduino.h>
#include <communication/communication.h>
#include <display/display.h>
#include <webService/webservice.h>

Communication com;
WebService web;
Display display;

void setup() {
    Serial.begin(115200);
    display.init();
    com.init();
    web.init();
}

void loop() {
    display.loop();
    com.loop();
    web.loop();

    delay(5);
}