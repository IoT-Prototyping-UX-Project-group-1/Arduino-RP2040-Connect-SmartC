#include <Arduino.h>
#include <SPI.h>
#include <WiFiNINA.h>

#include "board.hpp"
#include "network.hpp"

#define BTNLED_PIN 2     // D2
#define BUTTON_PIN 3     //  D2
#define LED_RING_PIN 16  // D4
#define BUZZER_PIN 7     // D6
#define VIBRATION_PIN A2 //

#define JOYSTICK_X_PIN 0
#define JOYSTICK_Y_PIN 1

void printNetworkInformation();
Board *board;

void setup()
{
  Serial.begin(9600);

  // // check for the WiFi module:
  // if (WiFi.status() == WL_NO_MODULE) {
  //   Serial.println("Communication with WiFi module has failed...!");
  // }

  // // attempt to connect to WiFi network:
  // int wifiStatus = WL_IDLE_STATUS;
  // while (wifiStatus != WL_CONNECTED) {
  //   char ssid[] = SSID;
  //   char pass[] = PASS;

  //   Serial.print("Attempting to connect to WPA SSID: ");
  //   Serial.println(ssid);

  //   wifiStatus = WiFi.begin(ssid, pass);
  //   delay(10000);
  // }

  // Serial.println("Connected to Wi-Fi network...");
  // printNetworkInformation();

  board = new Board(BUTTON_PIN, BTNLED_PIN, BUZZER_PIN, VIBRATION_PIN, JOYSTICK_X_PIN, JOYSTICK_Y_PIN, LED_RING_PIN, 24);
  board->setTimer(50 * 60, 10 * 60);
  board->startTimer();
}

void loop()
{
  auto timeLeft = board->checkTimer();
  char* timeLeftChar = (char*)calloc(16, sizeof(char));
  sprintf(timeLeftChar, "Time left: %02d", timeLeft);
  free(timeLeftChar);
  board->setDisplayLine(1, "Hello World!");
  board->setDisplayLine(2, timeLeftChar);
  auto buttonState = board->getButtonState();
  if (buttonState == ButtonType::DOUBLE_PRESS)
  {
    board->solidLedRing(0xFF0000);
    board->setDisplayLine(5, "TIMER PAUSED!");
    board->setDisplayLine(3, "timer==paused");
    board->pauseTimer();
  }
  else if (buttonState == ButtonType::SINGLE_PRESS)
  {
    board->solidLedRing(0x0000ff);
    Timer timer = board->timer;
    if (timer.isRunning || timer.isPaused) {
      board->setDisplayLine(5, "TIMER STOPPED!");
      board->setDisplayLine(3, "timer==stopped");
      board->stopTimer();
    }
    else {
      board->setDisplayLine(5, "TIMER STARTED!");
      board->setDisplayLine(3, "timer==running");
      board->startTimer();
    }
  }
  else if (buttonState == ButtonType::LONG_PRESS)
  {
    board->solidLedRing(0x00ff00);
  }
  else if (buttonState == ButtonType::NO_PRESS)
  {
    board->solidLedRing(0x000000);
  }
}
