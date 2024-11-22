#include "webservice.h"

#include "webService/html_ui.h"

const char WebService::s_content_enc[] PROGMEM = "Content-Encoding";

WebService::WebService() : server(80) {}
void WebService::init() {
    dnsRequest();
    server.begin();
}

void WebService::loop() {}

void WebService::setStaticContentCacheHeaders(
    AsyncWebServerResponse *response) {
    char tmp[12];  // Temporary buffer for generating cache headers

    response->addHeader(F("Cache-Control"), "no-cache");

    // Setting the ETag header (For example, using some value or checksum in
    // `tmp`)
    response->addHeader(F("ETag"),
                        tmp);  // Assuming `tmp` is filled with a value
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

    // server.onNotFound([](AsyncWebServerRequest *request) {
    //     request->send(200, "text/html", htmlContent);
    // });
}

void WebService::pageRequest() {}

void WebService::apiRequest() {}

void WebService::imageRequest() {
    server.on(  // Get Image
        "/image", HTTP_GET, [](AsyncWebServerRequest *request) {
            if (request->hasParam("name")) {
                String filename = "/" + request->getParam("name")->value();
                // if (SD.exists(filename)) {
                //     File file = SD.open(filename);
                //     request->send(SD, filename, "application/octet-stream");
                //     file.close();
                // } else {
                //     request->send(404, "text/plain", "File not found");
                // }
            } else {
                request->send(400, "text/plain", "File name not specified");
            }
        });
    server.on(  // Get Upload
        "/image", HTTP_POST,
        [](AsyncWebServerRequest *request) {
            request->send(200, "text/plain", "Upload completed");
        },
        [](AsyncWebServerRequest *request, String filename, size_t index,
           uint8_t *data, size_t len, bool final) {
            if (!index) {
                // If this is the start of a new file
                Serial.printf("Uploading file: %s\n", filename.c_str());
                // if (SD.exists("/" + filename)) {
                //     SD.remove("/" + filename);
                // }
                // File file = SD.open("/" + filename, FILE_WRITE);
                // if (!file) {
                //     Serial.println("Failed to open file for writing");
                //     return;
                // }
                // file.close();
            }
            // Write data to the file
            // File file = SD.open("/" + filename, FILE_APPEND);
            // if (file) {
            //     file.write(data, len);
            //     file.close();
            // }
            // if (final) {
            //     // If this is the end of the file
            //     Serial.printf("Upload finished: %s, size: %u bytes\n",
            //                   filename.c_str(), index + len);
            // }
        });

    server.on(  // Get Upload
        "/image", HTTP_DELETE, [](AsyncWebServerRequest *request) {
            if (request->hasParam("name")) {
                String filename = "/" + request->getParam("name")->value();
                // if (SD.exists(filename)) {
                //     SD.remove(filename);
                //     request->send(200, "text/plain", "File deleted
                //     successfully");
                // } else {
                //     request->send(404, "text/plain", "File not found");
                // }
            } else {
                request->send(400, "text/plain", "File name not specified");
            }
        });
}