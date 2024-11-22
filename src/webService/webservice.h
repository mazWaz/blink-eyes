#include <ESPAsyncWebServer.h>

#include "webService/html_ui.h"

class WebService {
   private:
    AsyncWebServer server;
    static const char s_content_enc[];  // Declaration of static const member
    void setStaticContentCacheHeaders(
        AsyncWebServerResponse *response);  // Method to set cache headers

    void dnsRequest();
    void pageRequest();
    void apiRequest();
    void imageRequest();
    void handleFileUpload(AsyncWebServerRequest *request, String filename,
                          size_t index, uint8_t *data, size_t len, bool final);

   public:
    WebService();
    void init();
    void loop();
};