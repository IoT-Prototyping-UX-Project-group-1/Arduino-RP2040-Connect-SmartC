#include "hardware/board.hpp"
#include "pico/bootrom.h"

Board::Board(
    uint8_t buttonPin,
    uint8_t buttonLedPin,
    uint8_t buzzerPin,
    uint8_t vibrationPin,
    uint8_t joystickXPin, uint8_t joystickYPin,
    uint8_t ledRingPin, uint8_t ledRingBulbs)
{
    button = new Button(buttonPin, buttonLedPin);
    buzzer = new Buzzer(buzzerPin);
    vibration = new Vibration(vibrationPin);
    ledRing = new LedRing(ledRingBulbs, ledRingPin);
    display = new Display();
    joystick = new Joystick(joystickXPin, joystickYPin);
}

ButtonType Board::getButtonState()
{
    return button->checkButtonState();
}

void Board::solidLedRing(const uint32_t &color)
{
    ledRing->solidColor(color);
}

void Board::flashLedRing(const uint32_t &color, const uint8_t &times, const uint8_t &waitTime)
{
    ledRing->flashColor(color, times, waitTime);
}

void Board::clearDisplay()
{
    display->clearDisplay();
}

void Board::displayTime(const uint8_t &lineNumber)
{
    unsigned long epochTime = WiFi.getTime();
    unsigned long hours = (epochTime % 86400L) / 3600 + /* UTC+1 */ 1;
    unsigned long minutes = (epochTime % 3600) / 60;
    unsigned long seconds = epochTime % 60;

    char *time = (char *)calloc(21, sizeof(char));
    sprintf(time, "Time: %02d:%02d:%02d", hours, minutes, seconds);

    display->setLineText(lineNumber, time);
    free(time);
}

void Board::displayDate(const uint8_t &lineNumber)
{
    time_t time = WiFi.getTime();
    struct tm *timeinfo = gmtime(&time);
    char *date = (char *)calloc(21, sizeof(char));
    sprintf(date, "Date: %02d/%02d/%04d", timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900);

    display->setLineText(lineNumber, date);
    free(date);
}

void Board::setDisplayLine(const uint8_t &lineNumber, const char *text)
{
    display->setLineText(lineNumber, text);
}

void Board::flushDisplayLine(const uint8_t &lineNumber)
{
    display->flushLine(lineNumber);
}

JoystickDirection Board::getJoystickDirection()
{
    return joystick->getDirection();
}

JoystickCoords Board::getJoystickCoords()
{
    return joystick->getCoords();
}

void Board::setTimer(const uint16_t &time, const uint16_t &pause)
{
    timer.timeBuffer = time;
    timer.pauseBuffer = pause;
}

void Board::startTimer() { timer.state = TIMER_STARTED; }
void Board::pauseTimer() { timer.state = TIMER_PAUSED; }
void Board::stopTimer() { timer.state = TIMER_STOPPED; }

uint8_t Board::checkTimer()
{
    auto timerState = timer.state;

    if (timerState == TIMER_STARTED)
    {
        if (timer.timeBuffer > 0)
        {
            timer.timeBuffer--;
            return timer.timeBuffer / 60; // return the time left in minutes
        }
        else // timer has finished the allocated time
        {
            display->setLineText(3, "ENJOY YOUR PAUSE!"); // TODO: speak with team about this.
            timer.state = TIMER_PAUSED;                   // automatically switch the timer to the pause bank.
            return 0;
        }
    }

    if (timerState == TIMER_PAUSED)
    {
        if (timer.pauseBuffer > 0)
        {
            timer.pauseBuffer--;
            return timer.pauseBuffer / 60;
        }
        else
        {
            timer.timesCompleted++;
            display->setLineText(3, "PAUSE IS OVER!"); // TODO: speak with team about this.
            timer.state = TIMER_STOPPED;
            return timer.timeBuffer / 60;
        }
    }
    if (timerState == TIMER_STOPPED)
    {
        buzzer->on();
        display->setLineText(6, "TIMER FINISHED!"); // TODO: speak with team about this.
        return 0;
    }

    return 0;
}

void Board::connectToWiFi(const char* ssid, const char* pass) {
    delay(2500);
    Serial.println("Trying to connect to Wi-Fi, please wait...");
    int wifiStatus = WL_IDLE_STATUS;
    do
    {
        Serial.print("Attempting to connect to SSID: ");
        Serial.println(ssid);
        Serial.print("The provided PASS is: ");
        Serial.println(pass);

        wifiStatus = WiFi.begin(ssid, pass);
        delay(10000);
    } while (wifiStatus != WL_CONNECTED);

    if (wifiStatus == WL_CONNECTED) { Serial.println("Connected to wifi"); }
}

void Board::setHttpClient(const char *host, const char *path, const uint16_t port)
{
    if (httpClient != NULL) delete httpClient;
    httpClient = new HttpClient(host, path, port);
}

char* Board::fetch(const uint32_t fetchSize) { return httpClient->fetch(fetchSize); }

Board::~Board()
{
    if (button != nullptr)
        delete button;
    if (buzzer != nullptr)
        delete buzzer;
    if (vibration != nullptr)
        delete vibration;
    if (ledRing != nullptr)
        delete ledRing;
    if (display != nullptr)
        delete display;
    if (joystick != nullptr)
        delete joystick;
    if (httpClient != nullptr)
        delete httpClient;
}
