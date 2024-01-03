#include "hardware/buzzer.hpp"

Buzzer::Buzzer(uint8_t pin)
{
    this->pin = pin;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}

void Buzzer::on(uint8_t power)
{
    analogWrite(pin, power);
}

void Buzzer::off()
{
    digitalWrite(pin, LOW);
}

Buzzer::~Buzzer()
{
    digitalWrite(pin, LOW);
    pinMode(pin, INPUT);
}
