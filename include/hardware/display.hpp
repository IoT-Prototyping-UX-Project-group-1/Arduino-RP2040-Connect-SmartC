#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <SPI.h>

class Display {
private:
    U8G2_SH1107_SEEED_128X128_F_HW_I2C *display = nullptr;
    uint16_t linePosition[9] = {0, 16, 32, 48, 64, 80, 96, 112, 128};

public:
    Display();
    void setLineText(const uint8_t &lineNumber, const char *text);
    void clearDisplay();
    void flushLine(const uint8_t &lineNumber);
    ~Display();
};
