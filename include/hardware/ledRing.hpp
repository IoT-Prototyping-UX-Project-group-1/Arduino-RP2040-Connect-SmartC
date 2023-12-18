#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class LedRing
{
public:
    LedRing(uint16_t numberOfPixels, uint16_t pin);
    void solidColor(const uint32_t color);
    void flashColor(const uint32_t color, const uint8_t &times, const uint16_t &waitTime);
    void displayWeatherInformation(const uint8_t &firstTempStateIndex, const uint8_t &firstWeatherStateIndex, const uint8_t &secTempStateIndex, const uint8_t &secWeatherStateIndex);
    void displayTimerState(const uint8_t &timerState);
    void displayFirstForecast(const uint8_t &tempStateIndex, const uint8_t &weatherStateIndex);
    void displaySecondForecast(const uint8_t &tempStateIndex, const uint8_t &weatherStateIndex);
    const uint32_t getTemperatureColor(const uint8_t tempStateIndex);
    const uint32_t getWeatherColor(const uint8_t weatherStateIndex);
    ~LedRing();

private:
    Adafruit_NeoPixel *ledRing = nullptr;
};
