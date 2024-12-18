#ifndef WEBSERVICE_H
#define WEBSERVICE_H
#include <ESPAsyncWebServer.h>

#include "storage/s_fs.h"
#include "webService/html_ui.h"

class WebService {
   private:
    SFS &_fs;
    AsyncWebServer server;

    static const char s_content_enc[];  // Declaration of static const member
    void setStaticContentCacheHeaders(
        AsyncWebServerResponse *response);  // Method to set cache headers

    void dnsRequest();
    void pageRequest();
    void routes();

   public:
    WebService(SFS &fs);
    static String processor(const String &var);
    void init();
    void loop();
};

#endif
