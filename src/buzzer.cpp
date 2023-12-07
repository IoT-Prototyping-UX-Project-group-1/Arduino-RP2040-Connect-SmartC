#include "buzzer.hpp"

Buzzer::Buzzer(uint8_t pin) {
    this->pin = pin;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}

void Buzzer::on() {
    digitalWrite(pin, HIGH);
}

void Buzzer::off() {
    digitalWrite(pin, LOW);
}

Buzzer::~Buzzer() {
    digitalWrite(pin, LOW);
    pinMode(pin, INPUT);
}

