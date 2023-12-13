#include "hardware/ledRing.hpp"

LedRing::LedRing(uint16_t numberOfPixels, uint16_t pin) {
    ledRing = new Adafruit_NeoPixel(numberOfPixels, pin, NEO_GRB + NEO_KHZ800);
    ledRing->begin();
    ledRing->show();
    ledRing->setBrightness(255);
}

void LedRing::solidColor(const uint32_t color) {
    if (ledRing != nullptr) {
        ledRing->fill(color);
        ledRing->show();
    }
}

void LedRing::flashColor(const uint32_t color, const uint8_t& times, const uint16_t& waitTime) {
    if (ledRing != nullptr) {
        for (uint8_t i = 0; i < times; i++) {
            ledRing->clear();
            ledRing->show();
            delay(waitTime);
            solidColor(color);
            delay(waitTime);
        }
    }
}
