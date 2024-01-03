#include "hardware/vibration.hpp"

Vibration::Vibration(uint8_t pin)
{
    this->pin = pin;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}

void Vibration::on(uint8_t power)
{
    analogWrite(pin, power);
}

void Vibration::off()
{
    analogWrite(pin, 0);
}

Vibration::~Vibration()
{
    analogWrite(pin, 0);
    pinMode(pin, INPUT);
}
