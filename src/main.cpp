#include <Arduino.h>
#include <SPI.h>
#include <WiFiNINA.h>

#include "hardware/board.hpp"
#include "software/weather.hpp"

#define BTNLED_PIN 2    // D2
#define BUTTON_PIN 3    // D2
#define LED_RING_PIN 16 // D4
#define BUZZER_PIN 7    // D6
#define VIBRATION_PIN A2

#define JOYSTICK_X_PIN 0
#define JOYSTICK_Y_PIN 1

// #define FETCH_INTERVAL 1 * 20 * 3 // 3 loop iterations = 1 real life second. 5 minutes = 5 * 60 * 3.
uint16_t FETCH_INTERVAL = 1 * 10 * 3;        // 3 loop iterations = 1 real life second. 5 minutes = 5 * 60 * 3.
uint16_t UPDATE_FETCH_INTERVAL = 5 * 60 * 3; // 3 loop iterations = 1 real life second. 5 minutes = 5 * 60 * 3.
uint16_t timeElapsedSinceLastFetch = 1 * 20 * 3;

Board *board;
WeatherInformation weatherInformation;

void setup()
{
  Serial.begin(9600);

  board = new Board(BUTTON_PIN, BTNLED_PIN, BUZZER_PIN, VIBRATION_PIN, JOYSTICK_X_PIN, JOYSTICK_Y_PIN, LED_RING_PIN, 20);
  board->connectToWiFi(SSID, PASS);
  board->setHttpClient("api.openweathermap.org", "/data/2.5/forecast?lat=55.606&lon=13.001&mode=json&units=metric&appid=5f53ebe2b43de119e4402e5056059c7a", 443);
}

void loop()
{
  timeElapsedSinceLastFetch++;
  if (timeElapsedSinceLastFetch % 3 == 0)
    Serial.println(String("Time elapsed since last weather information fetch: " + String(timeElapsedSinceLastFetch / 3) + "s"));

  if (timeElapsedSinceLastFetch >= FETCH_INTERVAL)
  {
    Serial.println("Resetting the refresh interval (performing HTTP request)...");
    timeElapsedSinceLastFetch = 0;
    char *newWeatherInformation = board->fetch();
    Serial.println("Weather information successfully fetched and converted.");

    convertOpenWeatherToWeatherInformationStruct(newWeatherInformation, weatherInformation);
    Serial.println(String("City: " + String(weatherInformation.city)));
  }
  uint8_t firstForecastTemp = weatherInformation.firstForecast.temperatureState;
  uint8_t firstForecastWeather = weatherInformation.firstForecast.weatherState;
  uint8_t secForecastTemp = weatherInformation.secondForecast.temperatureState;
  uint8_t secForecastWeather = weatherInformation.secondForecast.weatherState;

  board->displayWeatherInformation(firstForecastTemp, firstForecastWeather, secForecastTemp, secForecastWeather);
  board->displayTimerState(0);
  board->displayDate(6);
  board->displayTime(7);

  FETCH_INTERVAL = UPDATE_FETCH_INTERVAL;
}

void setup1()
{
  // delay(5000);
  Serial.printf("Core 1: Standing by...\n");
}

void loop1()
{
  // delay(12000);
  // Serial.println("Core 1: Waited for 12 seconds...");
}
