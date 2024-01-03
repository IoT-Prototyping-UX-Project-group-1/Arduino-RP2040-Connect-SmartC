#include "hardware/button.hpp"

Button::Button(uint8_t pin, uint8_t ledPin)
{
    this->pin = pin;
    this->ledPin = ledPin;
    pinMode(pin, INPUT_PULLDOWN);
    pinMode(ledPin, OUTPUT);
}

ButtonType Button::getButtonState()
{
    return this->buttonState;
}

void Button::updateButtonState()
{
    this->state = digitalRead(this->pin);

    if (this->state != this->lastState)
    {
        this->pressCount++;

        switch (buttonStateService())
        {
        case SINGLE_PRESS: /* From Unknown || Off states, a click turns the timer ON  && From Pause state (DOUBLE_PRESS), a click turns the timer back to ON*/
            serviceStateUpdate("Button ON !", HIGH, true, false, false, SINGLE_PRESS);
            break;
        case NO_PRESS: /* From ON state, a single click turns the timer OFF */
            serviceStateUpdate("Button OFF !", HIGH, true, false, true, NO_PRESS);
            break;
        case DOUBLE_PRESS: /* From ON state, a double click turns the timer on pause (DOUBLE_CLICK)*/
            serviceStateUpdate("Double Click!", LOW, true, false, false, DOUBLE_PRESS);
            break;
        case RESET: /* When releasing the push button, led turns OFF && from any state, a longer (> 5sec) click turns the timer on reset*/
            serviceStateUpdate("", LOW, false, true, false, RESET);
            this->buttonState = this->pressEnd - this->pressStart > this->RESET_INTERVAL ? RESET : this->buttonState;
            break;
        default:
            break;
        }

        this->lastState = this->state;
    }
}

void Button::serviceStateUpdate(String serialMessage, PinStatus ledPinStatus, bool isPressStart, bool isPressEnd, bool isDoubleClick, ButtonType buttonNewState)
{
    this->buttonState = buttonNewState != RESET ? buttonNewState : this->buttonState;
    Serial.println(serialMessage);
    digitalWrite(this->ledPin, ledPinStatus);
    this->pressStart = isPressStart ? millis() : this->pressStart;
    this->pressEnd = isPressEnd ? millis() : this->pressEnd;
    this->doubleClickIntervalStart = isDoubleClick ? millis() : this->doubleClickIntervalStart;
}

ButtonType Button::buttonStateService()
{
    return ((this->buttonState == UNKNOWN || this->buttonState == NO_PRESS) && this->pressCount % 2 == 0) || ((this->buttonState == DOUBLE_PRESS) && this->pressCount % 2 == 0) ? SINGLE_PRESS
           : ((this->buttonState == SINGLE_PRESS) && this->pressCount % 2 == 0)                                                                                                 ? NO_PRESS
           : (this->pressCount % 2 == 1 && this->buttonState == SINGLE_PRESS && millis() - this->doubleClickIntervalStart < this->DOUBLE_CLICK_INTERVAL)                        ? DOUBLE_PRESS
           : ((this->pressCount % 2 == 1) && this->pressEnd - this->pressStart > this->RESET_INTERVAL)                                                                          ? RESET
                                                                                                                                                                                : this->buttonState;
}

Button::~Button()
{
    pinMode(pin, INPUT);
}
