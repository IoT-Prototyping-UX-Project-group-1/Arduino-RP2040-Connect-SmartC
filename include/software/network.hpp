#include <Arduino.h>
#include <WiFiNINA.h>

// #define SSID "Turris"
// #define PASS "Omnia1234"

#define SSID "iOtE"
#define PASS "sgri*+x/esdfi17ao77#=3o"
class HttpClient
{
private:
  char *host = NULL;
  char *path = NULL;
  uint16_t port;

public:
  HttpClient(const char *host, const char *path, const uint16_t port = 80);
  char *fetch(const uint32_t fetchSize = 16 * 1024 /*process only ~16KB of data*/);
  ~HttpClient();
};
