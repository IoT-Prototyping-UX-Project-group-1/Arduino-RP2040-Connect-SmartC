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
    uint8_t timesCompleted;
    uint16_t timeBuffer;
    uint16_t pauseBuffer;
    TimerState state;
} Timer;

class Board
{
private:
    Button *button              = NULL;
    Buzzer *buzzer              = NULL;
    Vibration *vibration        = NULL;
    LedRing *ledRing            = NULL;
    Display *display            = NULL;
    Joystick *joystick          = NULL;
    HttpClient *httpClient      = NULL;

public:
    Timer timer;

    Board(
        uint8_t buttonPin, uint8_t buttonLedPin,
        uint8_t buzzerPin,
        uint8_t vibrationPin,
        uint8_t joystickXPin, uint8_t joystickYPin,
        uint8_t ledRingPin, uint8_t ledRingBulbs);

    /* Button methods */
    ButtonType getButtonState();

    /* LED Ring methods */
    void solidLedRing(const uint32_t &color);
    void flashLedRing(const uint32_t &color, const uint8_t &times, const uint8_t &waitTime);

    /* OLED Display methods */
    void clearDisplay();
    void displayTime(const uint8_t &lineNumber);
    void displayDate(const uint8_t &lineNumber);
    void setDisplayLine(const uint8_t &lineNumber, const char *text);
    void flushDisplayLine(const uint8_t &lineNumber);

    /* Joystick methods */
    JoystickDirection getJoystickDirection();
    JoystickCoords getJoystickCoords();

    /* Timer methods */
    void setTimer(const uint16_t &time, const uint16_t &pause);
    void startTimer();
    void pauseTimer();
    void stopTimer();
    uint8_t checkTimer();

    /* HTTP Client methods */
    void connectToWiFi(const char *ssid, const char *pass);
    void setHttpClient(const char *host, const char *path, const uint16_t port = 80);
    char* fetch(const uint32_t fetchSize = 16 * 1024 /*process only ~16KB of data*/);

    ~Board();
};
