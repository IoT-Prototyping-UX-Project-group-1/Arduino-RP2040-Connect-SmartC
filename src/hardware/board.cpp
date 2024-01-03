#include "hardware/board.hpp"
#include "pico/bootrom.h"

Board::Board(
    uint8_t buttonPin,
    uint8_t buttonLedPin,
    uint8_t buzzerPin,
    uint8_t vibrationPin,
    uint8_t ledRingPin, uint8_t ledRingBulbs)
{
    button = new Button(buttonPin, buttonLedPin);
    buzzer = new Buzzer(buzzerPin);
    vibration = new Vibration(vibrationPin);
    ledRing = new LedRing(ledRingBulbs, ledRingPin);
    display = new Display();
}

void Board::updateButtonState()
{
    button->updateButtonState();
}

ButtonType Board::getButtonState()
{
    return button->getButtonState();
}

void Board::solidLedRing(const uint32_t &color)
{
    ledRing->solidColor(color);
}

void Board::flashLedRing(const uint32_t &color, const uint8_t &times, const uint8_t &waitTime)
{
    ledRing->flashColor(color, times, waitTime);
}

void Board::displayWeatherInformation(float temp, char *weather, const uint8_t &firstTempStateIndex, const uint8_t &firstWeatherStateIndex, const uint8_t &secTempStateIndex, const uint8_t &secWeatherStateIndex)
{
    ledRing->displayWeatherInformation(firstTempStateIndex, firstWeatherStateIndex, secTempStateIndex, secWeatherStateIndex);

    char *tempDisplay = (char *)calloc(1, sizeof(char));
    char *weatherDisplay = (char *)calloc(21, sizeof(char));

    sprintf(tempDisplay, "  Temp: %02f°C", temp);
    sprintf(weatherDisplay, "  Weather: %s", weather);

    display->setLineText(1, "# Next 3 hours:");
    display->setLineText(2, tempDisplay);
    display->setLineText(3, weatherDisplay);

    free(tempDisplay);
    free(weatherDisplay);
}

void Board::displayTimerState(const uint8_t &timerState)
{
    ledRing->displayTimerState(timerState);
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

void Board::startTimer(uint8_t timerCounter)
{
    displayTimerState(timerCounter < (timer.oneCycle - timer.restInterval) ? 1 : 2);
}
void Board::pauseTimer()
{
    displayTimerState(3);
    buzzer->off();
    vibration->off();
}
void Board::stopTimer()
{
    displayTimerState(0);
    buzzer->off();
    vibration->off();
}

void Board::connectToWiFi(const char *ssid, const char *pass)
{
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

    if (wifiStatus == WL_CONNECTED)
    {
        Serial.println("Connected to wifi");
    }
}

void Board::setHttpClient(const char *host, const char *path, const uint16_t port)
{
    if (httpClient != NULL)
        delete httpClient;
    httpClient = new HttpClient(host, path, port);
}

char *Board::fetch(const uint32_t fetchSize) { return httpClient->fetch(fetchSize); }

void Board::onBuzzer(uint8_t power)
{
    buzzer->on(power);
}
void Board::offBuzzer()
{
    buzzer->off();
}

void Board::onVibrationMotor(uint8_t power)
{
    vibration->on(power);
}
void Board::offVibrationMotor()
{
    vibration->off();
}

void Board::onSignals(uint8_t power[2])
{
    vibration->on(power[0]);
    buzzer->on(power[1]);
}

void Board::reboot()
{
    // reset_usb_boot(1 << digitalPinToPinName(LED_BUILTIN), 0);
    _ontouch1200bps_();
}

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
    if (httpClient != nullptr)
        delete httpClient;
}
