#include <Arduino.h>

enum JoystickDirection {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    CENTER
};

typedef struct {
    uint16_t x;
    uint16_t y;
} JoystickCoords;

class Joystick {
private:
    uint8_t xPin;
    uint8_t yPin;
    JoystickCoords coords;
    uint16_t getX();
    uint16_t getY();

public:
    Joystick(uint8_t xPin, uint8_t yPin);
    JoystickDirection getDirection();
    JoystickCoords getCoords();
    ~Joystick();
};
