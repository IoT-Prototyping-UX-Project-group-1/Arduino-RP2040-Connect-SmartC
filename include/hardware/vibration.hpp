#include <Arduino.h>

class Vibration
{
private:
    uint8_t pin;
    uint8_t state;

public:
    Vibration(uint8_t pin);
    void on(uint8_t power);
    void off();
    ~Vibration();
};