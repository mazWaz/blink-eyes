#include <DNSServer.h>
#include <WiFi.h>

#include "config.h"

class Communication {
   private:
    DNSServer dnsServer;

   public:
    Communication();
    void loop();
    void init();
};
