#include "webservice.h"

#include "../storage/s_spiffs.h"  // Make sure the path to s_spiffs.h is correct
#include "webService/html_ui.h"

extern SSPIFFS spiffs;  // Declare the spiffs object here
extern SemaphoreHandle_t mutex;

const char WebService::s_content_enc[] PROGMEM = "Content-Encoding";

WebService::WebService(SFS &fs) : server(80), _fs(fs) {}
void WebService::init() {
    dnsRequest();
    pageRequest();
    routes();
    server.serveStatic("/", SD, "/");

    server.begin();
}

String WebService::processor(const String &var) {
    if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
        xSemaphoreGive(mutex);
        return String();
    }
}

void WebService::loop() {}

void WebService::setStaticContentCacheHeaders(
    AsyncWebServerResponse *response) {
    char tmp[12];
    response->addHeader(F("Cache-Control"), "no-cache");
    response->addHeader(F("ETag"), tmp);
}

void WebService::pageRequest() {
    server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request) {
        AsyncWebServerResponse *response;
        response = request->beginResponse_P(200, "text/html", PAGE_index,
                                            PAGE_index_L);

        response->addHeader(FPSTR(s_content_enc), "gzip");
        this->setStaticContentCacheHeaders(response);
        request->send(response);
    });

    server.on("/setting", HTTP_GET, [this](AsyncWebServerRequest *request) {
        AsyncWebServerResponse *response;
        response = request->beginResponse_P(200, "text/html", PAGE_index,
                                            PAGE_index_L);

        response->addHeader(FPSTR(s_content_enc), "gzip");
        this->setStaticContentCacheHeaders(response);
        request->send(response);
    });
}

void WebService::routes() {
    server.on("/list", HTTP_GET, [this](AsyncWebServerRequest *request) {
        String fileList = _fs.listFiles();
        request->send(200, "text/plain", fileList);
    });

    server.on("/set", HTTP_GET, [this](AsyncWebServerRequest *request) {
        if (request->hasParam("file")) {
            String filename = request->getParam("file")->value();
            if (_fs.fileExists(filename)) {
                if (spiffs.saveGif(filename)) {
                    request->send(200, "text/plain", "File Set successfully");
                    return;
                }
                request->send(404, "text/plain", "File Not Found Failed");

            } else {
                request->send(404, "text/plain", "File not found");
                return;
            }
        }
        request->send(400, "text/plain", "Filename not specified");
        return;
    });

    server.on("/get-wifi-ap", HTTP_GET, [this](AsyncWebServerRequest *request) {
        String setFile = spiffs.getWifiAp();
        if (setFile) {
            request->send(200, "text/plain", setFile);
        } else {
            request->send(404, "text/plain", "Not Found");
        }
    });

    server.on("/set-wifi-ap", HTTP_GET, [this](AsyncWebServerRequest *request) {
        if (request->hasParam("wifi_ap", true) &&
            request->hasParam("wifi_password", true)) {
            AsyncWebParameter *p_ap = request->getParam("wifi_ap", true);
            AsyncWebParameter *p_password =
                request->getParam("wifi_password", true);

            Serial.print("wifi AP: ");
            Serial.println(p_ap->value());
            Serial.print("wifi Pass: ");
            Serial.println(p_password->value());
            request->send(400, "text/plain", "Fail To Set");

            return;
        } else {
            request->send(400, "text/plain", "Fail To Set");

            return;
        }
    });

    server.on(
        "/upload", HTTP_POST, [this](AsyncWebServerRequest *request) {},
        [this](AsyncWebServerRequest *request, const String &filename,
               size_t index, uint8_t *data, size_t len, bool final) {
            if (!index) {
                request->_tempFile =
                    SD.open(("/image/" + filename).c_str(), FILE_WRITE);
            }
            if (len) {
                request->_tempFile.write(data, len);
            }
            if (final) {
                request->_tempFile.close();
                request->send(200, "text/plain", "Upload complete.");
            }
        });

    server.on(
        "/upload-cache", HTTP_POST, [this](AsyncWebServerRequest *request) {},
        [this](AsyncWebServerRequest *request, const String &filename,
               size_t index, uint8_t *data, size_t len, bool final) {
            if (!index) {
                request->_tempFile =
                    SD.open(("/cache/" + filename).c_str(), FILE_WRITE);
            }
            if (len) {
                request->_tempFile.write(data, len);
            }
            if (final) {
                request->_tempFile.close();
                if (_fs.fileExists(filename)) {
                    if (spiffs.saveGif(filename)) {
                        request->send(200, "text/plain", "Upload complete.");
                        return;
                    }
                }
                request->send(200, "text/plain", "Upload complete.");
            }
        });

    server.on("/delete", HTTP_GET, [this](AsyncWebServerRequest *request) {
        if (request->hasParam("file")) {
            String filename = request->getParam("file")->value();
            if (_fs.fileExists(filename)) {
                if (_fs.deleteFile(filename)) {
                    request->send(200, "text/plain",
                                  "File deleted successfully");
                    return;
                }
                request->send(404, "text/plain", "File not found1");
                return;
            } else {
                request->send(404, "text/plain", "File not found2");
                return;
            }
        }
        request->send(400, "text/plain", "Filename not specified");
        return;
    });
}

void WebService::dnsRequest() {
    server.on("/generate_204", HTTP_GET,
              [this](AsyncWebServerRequest *request) {
                  AsyncWebServerResponse *response = request->beginResponse_P(
                      200, "text/html", PAGE_index, PAGE_index_L);

                  response->addHeader(FPSTR(s_content_enc), "gzip");
                  this->setStaticContentCacheHeaders(response);
                  request->send(response);
              });

    server.on("/hotspot-detect.html", HTTP_GET,
              [this](AsyncWebServerRequest *request) {
                  AsyncWebServerResponse *response;
                  response = request->beginResponse_P(200, "text/html",
                                                      PAGE_index, PAGE_index_L);

                  response->addHeader(FPSTR(s_content_enc), "gzip");
                  this->setStaticContentCacheHeaders(response);
                  request->send(response);
              });

    server.on("/ncsi.txt", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", "Microsoft NCSI");
    });

    server.onNotFound([this](AsyncWebServerRequest *request) {
        AsyncWebServerResponse *response = request->beginResponse_P(
            200, "text/html", PAGE_index, PAGE_index_L);

        response->addHeader(FPSTR(s_content_enc), "gzip");
        this->setStaticContentCacheHeaders(response);
        request->send(response);
    });
}
