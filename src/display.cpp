#include "display.hpp"

Display::Display() {
    display = new U8G2_SH1107_SEEED_128X128_F_HW_I2C(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
    display->begin();
    display->setFont(u8g2_font_t0_14_tr);
    display->setFontRefHeightExtendedText();
    display->setDrawColor(1);
    display->setFontPosTop();
}

void Display::setLineText(const uint8_t &lineNumber, const char *text) {
    display->drawStr(0, linePosition[lineNumber], text);
    display->sendBuffer();
}

void Display::clearDisplay() {
    display->clear();
}

Display::~Display() {
    delete display;
}
