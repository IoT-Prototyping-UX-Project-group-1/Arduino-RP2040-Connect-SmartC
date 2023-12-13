#include <Arduino.h>
#include <WiFiNINA.h>

#define SSID "Turris"
#define PASS "Omnia1234"

typedef struct Time
{
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
};

enum WeatherType
{
  CLOUDS,
  RAIN,
  SNOW,
  CLEAR
};

typedef struct WeatherInformation
{
  Time time;
  float temperature;
  float humidity;
  WeatherType weatherType;
};

class HttpClient
{
private:
  char *host = NULL;
  char *path = NULL;
  uint16_t port;

public:
  HttpClient(const char *host, const char *path, const uint16_t port = 80);
  const char *fetch(const uint32_t fetchSize = 16*1024 /*process only ~16KB of data*/);
  ~HttpClient();
};
