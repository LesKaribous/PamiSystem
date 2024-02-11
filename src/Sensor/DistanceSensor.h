#pragma once 

#include <Arduino.h>
#include <Wire.h>
#include <VL53L0X.h>

#include "../Ihm/Ihm.h"

namespace Sensor{
    class DistanceSensor
    {
    private:
        VL53L0X _sensor;
        int _xshutPin;

    public:
        DistanceSensor() = default;
        DistanceSensor(int shutPin) : _xshutPin(shutPin) {};
        ~DistanceSensor() = default;

        inline void setShutPin(int shutPin) {_xshutPin = shutPin;}

        void init(int sensor_index = 0);
        void test();
    };
} // namespace Sensor