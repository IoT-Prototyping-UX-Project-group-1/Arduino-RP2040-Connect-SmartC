#include <Arduino.h>
#include <SPI.h>
#include <WiFiNINA.h>

#include "hardware/board.hpp"
#include "software/weather.hpp"

#define BTNLED_PIN 2     // D2 button led
#define BUTTON_PIN 3     // D2 button signal
#define LED_RING_PIN 16  // D4 LED Ring Stripe
#define BUZZER_PIN 6     // D6 Buzzer
#define VIBRATION_PIN A0 // Vibration Motor

// #define FETCH_INTERVAL 1 * 20 * 3 // 3 loop iterations = 1 real life second. 5 minutes = 5 * 60 * 3.
uint16_t FETCH_INTERVAL = 1 * 10 * 3;            // initialized with the first fetch -> updated after the first fetch
uint16_t UPDATE_FETCH_INTERVAL = 5 * 60 * 3;     // 3 loop iterations = 1 real life second. 5 minutes = 5 * 60 * 3.
uint16_t timeElapsedSinceLastFetch = 1 * 20 * 3; // fetching thw weather data
uint8_t timerCounter = 0;                        // the timer counter
uint8_t timerSuspendedTime = 0;                  // the timer suspended time

Board *board;
WeatherInformation weatherInformation;

void setup()
{
  Serial.begin(9600);

  board = new Board(BUTTON_PIN, BTNLED_PIN, BUZZER_PIN, VIBRATION_PIN, LED_RING_PIN, 20);
  board->connectToWiFi(SSID, PASS);
  board->setHttpClient("api.openweathermap.org", "/data/2.5/forecast?lat=55.606&lon=13.001&mode=json&units=metric&appid=5f53ebe2b43de119e4402e5056059c7a", 443);
}

void loop()
{
  board->updateButtonState();
  auto buttonState = board->getButtonState();
  if (buttonState == ButtonType::DOUBLE_PRESS)
  {
    board->timer.isRunning = false;
    board->timer.isStoped = true;
    board->timer.isSuspended = true;
    board->pauseTimer();
  }
  else if (buttonState == ButtonType::SINGLE_PRESS)
  {
    board->startTimer(timerCounter);
    board->onSignals((timerCounter < 1 || ((timerCounter > (board->timer.oneCycle - board->timer.restInterval)) && (timerCounter < (board->timer.oneCycle - board->timer.restInterval + 2)))) ? new uint8_t[2]{75, 5} : new uint8_t[2]{0, 0});
    board->timer.isRunning = true;
    board->timer.isSuspended = false;
    board->timer.isStoped = false;
    timerSuspendedTime = 0;
  }
  else if (buttonState == ButtonType::NO_PRESS)
  {
    timerSuspendedTime = timerSuspendedTime == 0 ? timerCounter : timerSuspendedTime;
    board->timer.isRunning = false;
    board->timer.isStoped = true;
    board->stopTimer();
  }
  else if (buttonState == ButtonType::RESET)
  {
    board->reboot();
  }

  timeElapsedSinceLastFetch++;
  if (timeElapsedSinceLastFetch % 3 == 0)
  {
    Serial.println(String("Time elapsed since last weather information fetch: " + String(timeElapsedSinceLastFetch / 3) + "s"));
    if (board->timer.isRunning)
    {
      timerCounter++;
      timerCounter %= board->timer.oneCycle;
    }
    if (board->timer.isSuspended)
    {
      timerSuspendedTime = timerCounter;
    }
    Serial.println(String("Timer Counter: " + String(timerCounter)));
    // Serial.println(String("Timer Suspended: " + String(timerSuspendedTime)));
  }
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

  Serial.println(String("Temperature: " + String(String(weatherInformation.feelsLike))));
  Serial.println(String("Weather: " + String(String(weatherInformation.weather))));

  board->displayWeatherInformation(weatherInformation.feelsLike, weatherInformation.weather, firstForecastTemp, firstForecastWeather, secForecastTemp, secForecastWeather);
  board->displayDate(6);
  board->displayTime(7);

  FETCH_INTERVAL = UPDATE_FETCH_INTERVAL;
}