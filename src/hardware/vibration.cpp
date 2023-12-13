#include "hardware/vibration.hpp"

Vibration::Vibration(uint8_t pin)
{
    this->pin = pin;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}

void Vibration::on()
{
    digitalWrite(pin, HIGH);
}

void Vibration::off()
{
    digitalWrite(pin, LOW);
}

Vibration::~Vibration()
{
    digitalWrite(pin, LOW);
    pinMode(pin, INPUT);
}
