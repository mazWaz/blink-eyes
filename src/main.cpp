#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include <Arduino.h>
#include <communication/communication.h>
#include <display/display.h>
#include <storage/s_fs.h>
#include <storage/s_spiffs.h>
#include <webService/webservice.h>

SemaphoreHandle_t mutex;

SSPIFFS spiffs;
Communication com;
SFS sd;
WebService web(sd);
Display display(sd);

TaskHandle_t Task1;
TaskHandle_t Task2;

void communiationTask(void *parameter);
void displayTask(void *parameter);
void setup()
{
    Serial.begin(115200);
    mutex = xSemaphoreCreateRecursiveMutex();

    spiffs.init();
    com.init();
    sd.init();
    web.init();
    display.init();
    xTaskCreatePinnedToCore(communiationTask, "Commnunication Task", 4096, NULL,
                            8, &Task1, 0);
    xTaskCreatePinnedToCore(displayTask, "Display Task", 32768, NULL, 2, &Task2,
                            1);
}

void loop()
{

    delay(5);
}

void communiationTask(void *parameter)
{
    while (true)
    {
        com.loop();
        delay(10);
    }
}

void displayTask(void *parameter)
{
    while (true)
    {
        display.loop();
        delay(10);
    }
}
