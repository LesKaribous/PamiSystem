#include "DistanceSensor.h"

namespace Sensor{

    void DistanceSensor::init(int sensor_index){
        // Disable/reset sensor by driving its XSHUT pin low.
        pinMode(_xshutPin, OUTPUT);
        digitalWrite(_xshutPin, LOW);

        // Stop driving this sensor's XSHUT low. This should allow the carrier
        // board to pull it high. (We do NOT want to drive XSHUT high since it is
        // not level shifted.) Then wait a bit for the sensor to start up.
        pinMode(_xshutPin, INPUT);
        delay(10);

        _sensor.setTimeout(500);
        if (!_sensor.init())
        {
            Serial.print("Failed to detect and initialize sensor ");
            Serial.println(sensor_index);
        }
        else{
            Serial.println("Distance Sensor");
            Serial.print(sensor_index);
            Serial.println("initialized");
        }

        // Each sensor must have its address changed to a unique value other than
        // the default of 0x29 (except for the last one, which could be left at
        // the default). To make it simple, we'll just count up from 0x2A.
        _sensor.setAddress(0x2A + sensor_index);

        _sensor.startContinuous(50);
    }


    void DistanceSensor::test(){
        uint16_t sensor = _sensor.readRangeContinuousMillimeters(); 

        if(_sensor.timeoutOccurred())
            Ihm::getInstance().debug("TIMEOUT");

        Ihm::getInstance().debug(String(sensor));
    }
}