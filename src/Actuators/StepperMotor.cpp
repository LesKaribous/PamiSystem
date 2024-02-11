#include "StepperMotor.h"

namespace Actuator{
    StepperMotor::StepperMotor(int en_pin, int dir_pin, int step_pin, int ms1_pin, int ms2_pin) : _en_pin(en_pin), _dir_pin(dir_pin), _step_pin(step_pin), _ms1_pin(ms1_pin), _ms2_pin(ms2_pin)
    {
        _currentStepMode = StepMode::SIXTEENTH_STEP;
        _stepMultiplier = 16;

        _motor = std::make_unique<AccelStepper>(AccelStepper::DRIVER, _step_pin, _dir_pin);
    }
    
    StepperMotor::~StepperMotor()
    {
    }

    void StepperMotor::init(){
        pinMode(_en_pin, OUTPUT);
        pinMode(_dir_pin, OUTPUT);
        pinMode(_step_pin, OUTPUT);
        pinMode(_ms1_pin, OUTPUT);
        pinMode(_ms2_pin, OUTPUT);

        setStepMode(StepMode::QUARTER_STEP);

        // Disable motor
        digitalWrite(_en_pin, HIGH);

        setMaxSpeed();
        setAcceleration();
    }

    void StepperMotor::setMaxSpeed(float _maxSpeed){
        _motor->setMaxSpeed(_maxSpeed);
    }

    void StepperMotor::setAcceleration(float _acceleration){
        _motor->setAcceleration(_acceleration);
    }

    void StepperMotor::setStepMode(StepMode mode){
        _currentStepMode = mode;
        switch(mode) {
            case EIGHTH_STEP:
                digitalWrite(_ms1_pin, LOW);
                digitalWrite(_ms2_pin, LOW);
                _stepMultiplier = 8;
                break;
            case HALF_STEP:
                digitalWrite(_ms1_pin, HIGH);
                digitalWrite(_ms2_pin, LOW);
                _stepMultiplier = 2;
                break;
            case QUARTER_STEP:
                digitalWrite(_ms1_pin, LOW);
                digitalWrite(_ms2_pin, HIGH);
                _stepMultiplier = 4;
                break;
            case SIXTEENTH_STEP:
                digitalWrite(_ms1_pin, HIGH);
                digitalWrite(_ms2_pin, HIGH);
                _stepMultiplier = 16;
                break;
        }
    }

    void StepperMotor::move(long relative){
        _motor->move(relative);
    }

    void StepperMotor::enable(){
        digitalWrite(_en_pin, LOW);
    }

    void StepperMotor::disable(){
        digitalWrite(_en_pin, HIGH);
    }

    void StepperMotor::update(){
        _motor->run();
    }

}