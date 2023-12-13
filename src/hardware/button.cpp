#include "hardware/button.hpp"

Button::Button(uint8_t pin, uint8_t ledPin)
{
    this->pin = pin;
    this->ledPin = ledPin;
    pinMode(pin, INPUT_PULLDOWN);
    pinMode(ledPin, OUTPUT);
}

ButtonType Button::checkButtonState()
{
    this->state = digitalRead(this->pin);

    if (this->state == LOW && this->lastState == HIGH)
    {
        Serial.println("Button pressed");
        this->pressCount++;
        delay(250);

        this->state = digitalRead(this->pin);
        if (this->state == LOW)
        {
            Serial.println("Button was held (SINGLE CLICK)");
            digitalWrite(this->ledPin, HIGH);
            this->lastState = this->state;
            return SINGLE_PRESS;
        } else {
            Serial.println("Button was released (DOUBLE CLICK)");
            digitalWrite(this->ledPin, LOW);
            for(uint8_t i = 0; i < 40; i++) {
                delay(50);
                this->state = digitalRead(this->pin);
                if (this->state == LOW) {
                    Serial.println("Button was pressed (DOUBLE CLICK)");
                    digitalWrite(this->ledPin, HIGH);
                    this->pressCount++;
                    this->lastState = this->state;
                    return DOUBLE_PRESS;
                }
            }

            this->lastState = this->state;
            return NO_PRESS;
        }
    } else if (this->state == HIGH && this->lastState == LOW) {
        Serial.println("Button was released (UNKNOWN STATE)");
        digitalWrite(this->ledPin, LOW);
        this->lastState = this->state;
        return NO_PRESS;
    } else if (this->state == LOW && this->lastState == LOW) {
        Serial.println("Button is being held... (LONG PRESS)");

        // wait for 10 seconds:
        for (uint8_t i = 0; i < 200; i++) {
            delay(50);
            this->state = digitalRead(this->pin);
            if (this->state == HIGH) {
                Serial.println("Button was released before 10 sec mark (LONG PRESS)");
                digitalWrite(this->ledPin, LOW);
                this->lastState = this->state;
                return NO_PRESS;
            }
        }

        Serial.println("Button was held for 10 seconds (LONG PRESS)");
        digitalWrite(this->ledPin, HIGH);
        this->lastState = this->state;
        return LONG_PRESS;
    }

    return NO_PRESS;
}


Button::~Button()
{
    pinMode(pin, INPUT);
}
