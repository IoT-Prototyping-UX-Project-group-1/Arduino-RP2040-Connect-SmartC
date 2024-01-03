#include <Arduino.h>
#include <WiFiNINA.h>
#include <time.h>

#include "button.hpp"
#include "buzzer.hpp"
#include "display.hpp"
#include "joystick.hpp"
#include "ledRing.hpp"
#include "vibration.hpp"
#include "../software/network.hpp"

enum TimerState
{
    TIMER_STARTED,
    TIMER_PAUSED,
    TIMER_STOPPED
};
typedef struct
{
    uint8_t oneCycle = 15;
    uint8_t restInterval = 5;
    bool isSuspended = false;
    bool isRunning = false;
    bool isStoped = false;
} Timer;

class Board
{
private:
    Button *button = NULL;
    Buzzer *buzzer = NULL;
    Vibration *vibration = NULL;
    LedRing *ledRing = NULL;
    Display *display = NULL;
    HttpClient *httpClient = NULL;

public:
    Timer timer;

    Board(
        uint8_t buttonPin, uint8_t buttonLedPin,
        uint8_t buzzerPin,
        uint8_t vibrationPin,
        uint8_t ledRingPin, uint8_t ledRingBulbs);

    /* Button methods */
    void updateButtonState();
    ButtonType getButtonState();

    /* LED Ring methods */
    void solidLedRing(const uint32_t &color);
    void flashLedRing(const uint32_t &color, const uint8_t &times, const uint8_t &waitTime);
    void displayWeatherInformation(float temp, char *weather, const uint8_t &firstTempStateIndex, const uint8_t &firstWeatherStateIndex, const uint8_t &secTempStateIndex, const uint8_t &secWeatherStateIndex);
    void displayTimerState(const uint8_t &timerState);

    /* OLED Display methods */
    void clearDisplay();
    void displayTime(const uint8_t &lineNumber);
    void displayDate(const uint8_t &lineNumber);
    void setDisplayLine(const uint8_t &lineNumber, const char *text);
    void flushDisplayLine(const uint8_t &lineNumber);

    /* Timer methods */
    void startTimer(uint8_t oneCycle);
    void pauseTimer();
    void stopTimer();

    /* Buzzer methods */
    void onBuzzer(uint8_t power);
    void offBuzzer();

    void onSignals(uint8_t power[2]);

    /* Vibration Motor methods */
    void onVibrationMotor(uint8_t power);
    void offVibrationMotor();

    /* HTTP Client methods */
    void connectToWiFi(const char *ssid, const char *pass);
    void setHttpClient(const char *host, const char *path, const uint16_t port = 80);
    char *fetch(const uint32_t fetchSize = 16 * 1024 /*process only ~16KB of data*/);

    /* Reset Method */
    void reboot();

    ~Board();
};
