#pragma once

#include <Arduino.h>
#include <U8g2lib.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>

#include <Pami.h>

#include "pins.h" 

class Ihm
{
private:
    void initLCD();
    void initLedStatus();

    Adafruit_NeoPixel led;

    Robot::Pami *_robot;

public:
    Ihm();
    Ihm(Robot::Pami &robot);
    ~Ihm() = default;

    // Ihm should not be clonable
    Ihm(Ihm &) = delete;    

    // Ihm should not be assignable
    void operator=(const Ihm &) = delete;

    Robot::Pami& setRobot(Robot::Pami &robot);

    static Ihm &getInstance();

    void init();

    /* Preset screen displays */
    void drawSplashScreen();
    void pairingScreen();
    void drawBackLcd(); 
    void printMacAdress();

    /* Display methods */
    void debug(String message);
    void debugLCD(String message, u8g2_uint_t _y = 31);   // Affiche un message sur la ligne de debug (dessous)
    void infoLCD(String message,  u8g2_uint_t _y = 13);   // Affiche un message sur la ligne info (dessus)
    void drawLCD(String message, u8g2_uint_t _x, u8g2_uint_t _y); // Affiche un message et clear l'écran uniquement autour du message

    Robot::Team checkColorTeam();
};
