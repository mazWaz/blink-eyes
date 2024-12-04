#include <Arduino.h>
#include <communication/communication.h>
#include <display/display.h>
#include <webService/webservice.h>

Communication com;
SFS sd;
WebService web(sd);
Display display;

void setup() {
    Serial.begin(115200);
    sd.init();
    com.init();
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