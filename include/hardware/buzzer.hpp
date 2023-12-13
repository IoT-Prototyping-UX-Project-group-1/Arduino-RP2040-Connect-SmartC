#include <Arduino.h>

class Buzzer {
private:
    uint8_t pin;
    uint8_t state;

public:
    Buzzer(uint8_t pin);
    void on();
    void off();
    ~Buzzer();
};
