#include <Arduino.h>

class Buzzer {
private:
    uint8_t pin;
    uint8_t state;

public:
    Buzzer(uint8_t pin);
    void on(uint8_t power);
    void off();
    ~Buzzer();
};
