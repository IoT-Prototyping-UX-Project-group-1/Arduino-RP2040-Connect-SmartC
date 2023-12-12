#include "joystick.hpp"

Joystick::Joystick(uint8_t xPin, uint8_t yPin) {
    this->xPin = xPin;
    this->yPin = yPin;
}

uint16_t Joystick::getX() { return analogRead(xPin); }

uint16_t Joystick::getY() { return analogRead(yPin); }

JoystickDirection Joystick::getDirection() {
    uint16_t x = getX();
    uint16_t y = getY();
    uint16_t middleLowerLimit = 450;
    uint16_t middleUpperLimit = 550;

    // When X and Y are in the middle, return CENTER
    if (x >= middleLowerLimit && x <= middleUpperLimit &&
         y >= middleLowerLimit && y <= middleUpperLimit) {
        return CENTER;
    }

    // When X is in the middle, return UP or DOWN
    if (x >= middleLowerLimit && x <= middleUpperLimit) {
        if (y < middleLowerLimit) { return UP; }
        else if (y > middleUpperLimit) { return DOWN; }
    }

    // When Y is in the middle, return LEFT or RIGHT
    if (y >= middleLowerLimit && y <= middleUpperLimit) {
        if (x < middleLowerLimit) { return LEFT; }
        else if (x > middleUpperLimit) { return RIGHT; }
    }

    return CENTER;
}

JoystickCoords Joystick::getCoords() {
    coords.x = getX();
    coords.y = getY();
    return coords;
}

Joystick::~Joystick() {
    // Nothing to do here
}
