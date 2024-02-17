#pragma once

#include <Arduino.h>
#include <Servo.h>


#include "pins.h"


#define ANTENNA_UP 120
#define ANTENNA_DOWN 45

namespace Actuator{
    class Antenna
    {
    private:
        Servo servoAntenna;
        int frequency = 200;
    public:
        Antenna() = default;
        ~Antenna() = default;

        void init(int pin);
        void down();
        void up();
        void test();
    };

} // namespace Actuator
