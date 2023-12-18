#include <Arduino.h>
enum ButtonType {
    NO_PRESS,
    SINGLE_PRESS,
    DOUBLE_PRESS,
    LONG_PRESS
};

class Button {
private:
    uint8_t pin;
    uint8_t ledPin;

public:
    uint8_t state = LOW;
    uint8_t lastState = LOW;
    uint16_t pressCount = 0;

    Button(uint8_t pin, uint8_t ledPin);
    ButtonType checkButtonState();
    ~Button();
};
