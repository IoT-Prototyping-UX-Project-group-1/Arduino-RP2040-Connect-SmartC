#include <Arduino.h>
#include <Adafruit_NeoPixel.h>


class LedRing {
public:
    LedRing(uint16_t numberOfPixels, uint16_t pin);
    void solidColor(const uint32_t color);
    void flashColor(const uint32_t color, const uint8_t &times, const uint16_t &waitTime);
    ~LedRing();

private:
    Adafruit_NeoPixel *ledRing = nullptr;
};
