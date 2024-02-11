#include "Antenna.h"

namespace Actuator{

    void Antenna::init(int pin){
        servoAntenna.attach(
            pin,
            Servo::CHANNEL_NOT_ATTACHED, 
            Servo::DEFAULT_MIN_ANGLE, 
            Servo::DEFAULT_MAX_ANGLE, 
            Servo::DEFAULT_MIN_PULSE_WIDTH_US, 
            Servo::DEFAULT_MAX_PULSE_WIDTH_US,
            frequency
        );
    }

    void Antenna::up(){
        servoAntenna.write(ANTENNA_UP);
        Ihm::getInstance().debug("Antenna 120 (up)");
    }

    void Antenna::down(){
        servoAntenna.write(ANTENNA_DOWN);
        Ihm::getInstance().debug("Antenna 45 (down)");
    }

    void Antenna::test(){
        down();
        delay(1000);
        up();
        delay(1000);
    }
}