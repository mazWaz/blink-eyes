#include "webservice.h"

#include "webService/html_ui.h"

const char WebService::s_content_enc[] PROGMEM = "Content-Encoding";

WebService::WebService(SFS &fs) : server(80), _fs(fs) {}
void WebService::init() {
    dnsRequest();
    pageRequest();
    imageRequest();
    server.serveStatic("/", SD, "/");

    server.begin();
}

void WebService::loop() {}

void WebService::setStaticContentCacheHeaders(
    AsyncWebServerResponse *response) {
    char tmp[12];
    response->addHeader(F("Cache-Control"), "no-cache");
    response->addHeader(F("ETag"), tmp);
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

void WebService::pageRequest() {
    server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request) {
        AsyncWebServerResponse *response;
        response = request->beginResponse_P(200, "text/html", PAGE_index,
                                            PAGE_index_L);

        response->addHeader(FPSTR(s_content_enc), "gzip");
        this->setStaticContentCacheHeaders(response);
        request->send(response);
    });
}

void WebService::apiRequest() {}

void WebService::imageRequest() {
    server.on("/list", HTTP_GET, [this](AsyncWebServerRequest *request) {
        String fileList = _fs.listFiles();
        request->send(200, "text/plain", fileList);
    });

    server.on("/image", HTTP_GET, [this](AsyncWebServerRequest *request) {
        if (request->hasParam("filename")) {
            String filename = request->getParam("filename")->value();
            if (_fs.exists("/" + filename)) {
                File file = SD.open(filename);
                AsyncWebServerResponse *response =
                    request->beginResponse(SD, filename, "image/gif");
                response->addHeader("Content-Disposition",
                                    "inline; filename=" +
                                        request->getParam("filename")->value());
                request->send(response);
                file.close();
            } else {
                request->send(404, "text/plain", "File not found");
            }
        } else {
            request->send(400, "text/plain", "Filename not specified");
        }
    });

    server.on("/upload", HTTP_GET, [this](AsyncWebServerRequest *request) {

    });

    server.on("/delete", HTTP_GET, [this](AsyncWebServerRequest *request) {
        if (request->hasParam("filename")) {
            String filename = request->getParam("filename")->value();
            if (_fs.deleteFile(filename)) {
                request->send(200, "text/plain", "File deleted successfully");
            } else {
                request->send(404, "text/plain", "File not found");
            }
        } else {
            request->send(400, "text/plain", "Filename not specified");
        }
    });
}