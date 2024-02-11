#pragma once 

#include <memory.h>

#include <Arduino.h>
#include <AccelStepper.h>

#include "../Actuators/Antenna.h"
#include "../Sensor/DistanceSensor.h"
#include "../Ihm/Ihm.h"
#include "../Actuators/StepperMotor.h"

#include "EspNowPami.h"
#include "Control.h"
#include "pins.h" 

#define WHEEL_DIAMETER_MM 60.0f     // Diamètre de la roue en millimètres
#define WHEEL_DISTANCE_MM 85.0f     // Distance entre les roues en millimètres

namespace Robot{
    enum State{
            WAIT,
            RUN,
            END,
            UNDEFINED = 99,
            PAIRING = 42,
            PAIRED = 60,
            ARMED = 70,
            READY=  80,
            START_MATCH = 126,
            MATCH_STARTED = 150,
        };

    enum Team{
        BLUE,
        YELLOW,
        UNDEFINED
    };

    class Pami
    {
    private:
        // Position absolue du robot
        Pose currentPose  = {0.0f, 0.0f, 0.0f};
        Pose targetPose   = {0.0f, 0.0f, 0.0f};

        // Déplacement cible polaire
        PolarMove targetMove = {0.0f, 0.0f, 0.0f};
        bool newPolarTarget = false;

        // Motors 
        std::unique_ptr<Actuator::StepperMotor> motor_D;
        std::unique_ptr<Actuator::StepperMotor> motor_G;

        // Distance sensors
        Sensor::DistanceSensor distance_sensors[2];

        // Antenne 
        Actuator::Antenna antenna;

        // EspNow
        Com::EspNowPami _esp; 

        // Ihm
        Ihm _ihm;

        // Robot's info
        State _state;
        Team _team;
        
        byte _id; // Robot's ID

        // Compule wheel circumference
        float _circumferenceMM = WHEEL_DIAMETER_MM * PI;

    protected:
        // Converti la position demandée vers le targetPolarMove
        void convertToPolar(Pose _target);
        void convertToPolar(float _x, float _y);
        void convertToPolar(float _x, float _y, float _rot);

        long convertDistToStep(float _dist);
        long convertAngleToStep(float angle);

        void readRobotID();

    public:
        Pami(/* args */);
        ~Pami();

        void init();

        inline byte getID() const {return _id;}

        inline State getState() const {return _state;}
        inline void setState(const State &state){_state = state;}

        inline Team getTeam() const {return _team;}
        inline void setTeam(const Team &team){_team = team;}
        
        void enableMotors();
        void disableMotors();
        void updateMotors();

        void processMove();

        // Déplacements relatifs
        void go(float _dist);
        void turn(float _angle);
        void turnGo(float _angle, float _dist);

        // Déplacements absolus
        void goTo(Pose _target);
        void goTo(float _x, float _y);
        void goTo(float _x, float _y, float _rot);

        inline bool getTirette() const {return !digitalRead(Pin::Tirette);}

        bool initEspNow();
    };

    extern Pami *robot;
}


