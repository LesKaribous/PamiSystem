#ifndef IHM_H
#define IHM_H

#include <Arduino.h>
#include <U8g2lib.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>

#include "pins.h" 

namespace Robot{class Pami; enum class State; enum class Team;}

class Ihm
{
public:
    Ihm();
    ~Ihm() = default;

    // Ihm should not be clonable
    Ihm(Ihm &) = delete;

    enum DEBUG{
        SERIALD,
        LCD
    };

    // Ihm should not be assignable
    void operator=(const Ihm &) = delete;

    static Ihm &getInstance();

    void init(const DEBUG &debug);

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

private:
    void initLCD();
    void initLedStatus();

    Adafruit_NeoPixel led;

    DEBUG debug_mode;
};

#endif