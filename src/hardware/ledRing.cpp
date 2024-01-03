#include "hardware/ledRing.hpp"

// Adafruit Neo-Pixel - 32-bit color values
const uint32_t BLACK = 0x000000;     // OFF
const uint32_t BLUE = 0x0000FF;      // FROST
const uint32_t GREEN = 0x008000;     // WORK -PRODUCTIVITY
const uint32_t LIME = 0x00FF00;      // MISTY
const uint32_t CYAN = 0x00FFFF;      // SNOWY
const uint32_t ROYALBLUE = 0x4169E1; // FEW_CLOUDS
const uint32_t STEELBLUE = 0x4682B4; // CLOUDY
const uint32_t MEDIUMTURQUOISE = 0x48D1CC;
const uint32_t INDIGO = 0x4B0082;     // LIGHT_RAIN
const uint32_t AQUAMARINE = 0x7FFFD4; // CLEAR_SKY
const uint32_t MAROON = 0x800000;     // HEAVY_RAIN
const uint32_t PURPLE = 0x800080;     // ALERT - STATE CHANGE
const uint32_t BLUEVIOLET = 0x8A2BE2; // RAINY
const uint32_t YELLOWGREEN = 0x9ACD32;
const uint32_t GREENYELLOW = 0xADFF2F; // PAUSE
const uint32_t VIOLET = 0xEE82EE;
const uint32_t RED = 0xFF0000;
const uint32_t MAGENTA = 0xFF00FF;
const uint32_t DEEPPINK = 0xFF1493;
const uint32_t ORANGERED = 0xFF4500; // HOT
const uint32_t TOMATO = 0xFF6347;
const uint32_t HOTPINK = 0xFF69B4;
const uint32_t ORANGE = 0xFFA500;
const uint32_t PINK = 0xFFC0CB; // SUSPENDED
const uint32_t GOLD = 0xFFD700;
const uint32_t YELLOW = 0xFFFF00;      // WARM
const uint32_t LIGHTYELLOW = 0xFFFFE0; // MODERATE
const uint32_t WHITE = 0xFFFFFF;       // COLD

// Adafruit Neo-Pixel - 32-bit color values
// Timer States
const uint32_t PRODUCTIVITY = 0x008000; // GREEN
const uint32_t OFF = 0x000000;          // BLACK
const uint32_t PAUSE = 0xFF4500;        // ORANGERED
const uint32_t SUSPENDED = 0xFF1493;    // DEEPPINK

// Temperature States
const uint32_t FROST = 0x0000FF;    // BLUE
const uint32_t COLD = 0xFFFFFF;     // WHITE
const uint32_t MODERATE = 0xFFFFE0; // LIGHTYELLOW
const uint32_t WARM = 0xFFFF00;     // YELLOW
const uint32_t HOT = 0xFF4500;      // ORANGERED

// Weather States
const uint32_t CLEAR_SKY = 0x7FFFD4;  // AQUAMARINE
const uint32_t FEW_CLOUDS = 0x4169E1; // ROYALBLUE
const uint32_t CLOUDY = 0x4682B4;     // STEELBLUE
const uint32_t LIGHT_RAIN = 0x4B0082; // INDIGO
const uint32_t RAINY = 0x8A2BE2;      // BLUEVIOLET
const uint32_t HEAVY_RAIN = 0x800000; // MAROON
const uint32_t MISTY = 0x00FF00;      // LIME
const uint32_t SNOWY = 0x7FFFD4;      // AQUAMARINE

LedRing::LedRing(uint16_t numberOfPixels, uint16_t pin)
{
    ledRing = new Adafruit_NeoPixel(numberOfPixels, pin, NEO_GRB + NEO_KHZ800);
    ledRing->begin();
    ledRing->show();
    ledRing->setBrightness(255);
}

void LedRing::solidColor(const uint32_t color)
{
    if (ledRing != nullptr)
    {
        ledRing->fill(color);
        ledRing->show();
    }
}

void LedRing::flashColor(const uint32_t color, const uint8_t &times, const uint16_t &waitTime)
{
    if (ledRing != nullptr)
    {
        for (uint8_t i = 0; i < times; i++)
        {
            ledRing->clear();
            ledRing->show();
            delay(waitTime);
            solidColor(color);
            delay(waitTime);
        }
    }
}

void LedRing::displayWeatherInformation(const uint8_t &firstTempStateIndex, const uint8_t &firstWeatherStateIndex, const uint8_t &secTempStateIndex, const uint8_t &secWeatherStateIndex)
{

    displayFirstForecast(firstTempStateIndex, firstWeatherStateIndex);
    displaySecondForecast(secTempStateIndex, secWeatherStateIndex);
}

void LedRing::displayTimerState(const uint8_t &timerState)
{
    switch (timerState)
    {
    case 0: // OFF state
        ledRing->setPixelColor(18, OFF);
        break;
    case 1: // ON state
        ledRing->setPixelColor(18, PRODUCTIVITY);
        break;
    case 2: // PAUSE state
        ledRing->setPixelColor(18, PAUSE);
        break;
    case 3: // SUSPENDED state
        ledRing->setPixelColor(18, SUSPENDED);
        break;
    default:
        break;
    }
    ledRing->setBrightness(10);
    ledRing->show();
}

void LedRing::displayFirstForecast(const uint8_t &tempStateIndex, const uint8_t &weatherStateIndex)
{
    const uint32_t tempColor = getTemperatureColor(tempStateIndex);
    const uint32_t weatherColor = getWeatherColor(weatherStateIndex);
    ledRing->setPixelColor(19, tempColor);
    ledRing->setPixelColor(0, tempColor);
    for (uint8_t i = 1; i < 9; i++)
    {
        ledRing->setPixelColor(i, weatherColor);
    }
    ledRing->setBrightness(5);
    ledRing->show();
}

void LedRing::displaySecondForecast(const uint8_t &tempStateIndex, const uint8_t &weatherStateIndex)
{
    const uint32_t tempColor = getTemperatureColor(tempStateIndex);
    const uint32_t weatherColor = getWeatherColor(weatherStateIndex);
    ledRing->setPixelColor(9, tempColor);
    ledRing->setPixelColor(10, tempColor);
    for (uint8_t i = 11; i < 18; i++)
    {
        ledRing->setPixelColor(i, weatherColor);
    }
    ledRing->setBrightness(5);
    ledRing->show();
}

const uint32_t LedRing::getTemperatureColor(const uint8_t tempStateIndex)
{
    switch (tempStateIndex)
    {
    case 0:
        return FROST;
        break;
    case 1:
        return COLD;
        break;
    case 2:
        return MODERATE;
        break;
    case 3:
        return WARM;
        break;
    case 4:
        return HOT;
        break;
    default:
        break;
    }
    return HOT;
}

const uint32_t LedRing::getWeatherColor(const uint8_t weatherStateIndex)
{
    switch (weatherStateIndex)
    {
    case 0:
        // return CLEAR_SKY;
        return GOLD;
        break;
    case 1:
        return FEW_CLOUDS;
        break;
    case 2:
        return CLOUDY;
        break;
    case 3:
        return LIGHT_RAIN;
        break;
    case 4:
        return RAINY;
        break;
    case 5:
        return HEAVY_RAIN;
        break;
    case 6:
        return MISTY;
        break;
    case 7:
        return SNOWY;
        break;
    default:
        break;
    }
    return SNOWY;
}