
#pragma once

#include <memory>

#include <Arduino.h>
#include <AccelStepper.h>

#define STEPS_PER_REVOLUTION 200    // Nombre de pas par tour du moteur
#define MAX_SPEED       20000.0
#define ACCELERATION    5000.0

namespace Actuator{
    class StepperMotor
    {
    private:
        enum StepMode {
            EIGHTH_STEP,      // MS1 = GND, MS2 = GND
            HALF_STEP,        // MS1 = GND, MS2 = VIO
            QUARTER_STEP,     // MS1 = VIO, MS2 = GND
            SIXTEENTH_STEP    // MS1 = VIO, MS2 = VIO
        };

        std::unique_ptr<AccelStepper> _motor;

        int _en_pin, _dir_pin, _step_pin, _ms1_pin, _ms2_pin;

        StepMode _currentStepMode;
        int _stepMultiplier;

    public:
        StepperMotor(int en_pin, int dir_pin, int step_pin, int ms1_pin, int ms2_pin);
        ~StepperMotor();

        void init();

        void setMaxSpeed(float _maxSpeed = MAX_SPEED);
        void setAcceleration(float _acceleration = ACCELERATION);
    
        void setStepMode(StepMode mode); 
        void enable();
        void disable();
        void update();

        void move(long relative);

        inline bool isRunning(){return _motor->isRunning();}

        inline void setStepMultiplier(int stepMultiplier){_stepMultiplier = stepMultiplier;}
        inline int getStepMulitplier() const {return _stepMultiplier;}
    };
    
} // namespace Actuator