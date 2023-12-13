#include <Arduino.h>
#include <SPI.h>
#include <WiFiNINA.h>

#include "board.hpp"

#define BTNLED_PIN 2     // D2
#define BUTTON_PIN 3     // D2
#define LED_RING_PIN 16  // D4
#define BUZZER_PIN 7     // D6
#define VIBRATION_PIN A2

#define JOYSTICK_X_PIN 0
#define JOYSTICK_Y_PIN 1

#define FETCH_INTERVAL 5 * 60 * 3 // 3 loop iterations = 1 real life second. 5 minutes = 5 * 60 * 3.
uint16_t timeElapsedSinceLastFetch = 5 * 60 * 3;

Board *board;
WeatherInformation *weatherInformation = NULL;

void setup()
{
  Serial.begin(9600);

  board = new Board(BUTTON_PIN, BTNLED_PIN, BUZZER_PIN, VIBRATION_PIN, JOYSTICK_X_PIN, JOYSTICK_Y_PIN, LED_RING_PIN, 24);
  board->connectToWiFi(SSID, PASS);
  board->setHttpClient("api.openweathermap.org", "/data/2.5/forecast?lat=55.606&lon=13.001&mode=json&units=metric&appid=5f53ebe2b43de119e4402e5056059c7a", 443);
}

void loop()
{
  timeElapsedSinceLastFetch++;

  if (timeElapsedSinceLastFetch % 3 == 0 ) Serial.println(String("Time elapsed since last weather information fetch: " + String(timeElapsedSinceLastFetch / 3) + "s"));
  if (timeElapsedSinceLastFetch >= FETCH_INTERVAL)
  {
    Serial.println("Resetting the refresh interval...");
    timeElapsedSinceLastFetch = 0;
    const char *newWeatherInformation = board->fetch();
    if (newWeatherInformation != NULL)
    {
      if (weatherInformation != NULL)
        free(weatherInformation);
      
    }
  }

  board->displayDate(6);
  board->displayTime(7);
}
