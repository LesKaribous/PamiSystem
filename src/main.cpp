#include <Arduino.h>

#include "Pami/Pami.h"

#define PAMI_WAIT 0
#define PAMI_RUN 1
#define PAMI_END 2

#define TIME_START_PAMI 0
#define TIME_END_PAMI 10000

#define MATCH_TIME 100000

//TaskHandle_t Task1;

long elapsedTime  = 0;
long startTime    = 0;
int  statePAMI = PAMI_WAIT;

void waitStart();
void updateMatchTime();
void testMatch();

Robot::Pami pami;

void setup() {

  pami.init();

  // initIHM();
  // initSensor();
  // initMotion();
  // initActuators();

  // drawSplashScreen();
  // pairingScreen();
  // drawBackLcd();

  // enableMotors();
  // antennasUp();

  waitStart();
}

void loop() 
{
  //updateMatchTime();
  //testAntennas();
  //testSensor();
  //testMatch();
  
  pami.enableMotors();
  
  //turn(3600);
  //delay(1000);
  
  pami.goTo(100,0,0);
  pami.goTo(0,0,0);
  
  /*
  goTo(100,100,90);
  goTo(100,100,180);
  goTo(100,0,0);
  */
  
  /*
  go(100);
  turn(90);
  turn(-90);
  go(-100);
  */
}

void waitStart(){
  // //Attendre que la tirette n'est soit plus présente
  // infoLCD("Remove Tirette");
  // while(pami.getTirette()) {
  //   delay(250); 
  //   checkColorTeam();
  // }
  // //Attendre que la tirette soit insérée
  // infoLCD("Insert Tirette");
  // while(!pami.getTirette() && pami.getState() != Pami::State::READY) {
  //   delay(250); 
  //   checkColorTeam();
  // }
  // // Si la tirette a été insérée
  // // Le robot va être lancés avec la tirette
  // // Les message de démarrage seront envoyé sur le réseau
  // // Seul le PAMI 1 envoi les messages
  // if(getTirette()){
  //   BroadcastMessage(Pami::State::ARMED);
  //   setRobotState(Pami::State::READY);
  //   infoLCD("Wait Start");
  //   //Attendre que la tirette soit retirée
  //   while(getTirette())
  //   {
  //     delay(250); 
  //     checkColorTeam();
  //   }
  //   BroadcastMessage(Pami::State::START_MATCH);
  //   setRobotState(Pami::State::MATCH_STARTED);
  // }
  // else
  // {
  //   // Cas des PAMIS 2 et 3 qui attendent le lancement du match sans tirette
  //   infoLCD("Wait Start");
  //   //Attendre l'ordre du PAMI 1
  //   while(pami.getState() != Pami::State::MATCH_STARTED)
  //   {
  //     delay(250); 
  //     checkColorTeam();
  //   }
  // }
  // infoLCD("Start Robot");
  // // Démarrage du compteur !
  // startTime = millis();
}

void updateMatchTime(){
  elapsedTime = millis() - startTime ;
  if(elapsedTime>= TIME_START_PAMI)   statePAMI = true;
  if(elapsedTime>= TIME_END_PAMI)     statePAMI = false;
}

void testMatch(){
  if(statePAMI == PAMI_RUN){
    pami.enableMotors();
    // ToDo - Ecrire la strat
  }
  else if (statePAMI == PAMI_END){
    pami.disableMotors(); // Desactive les moteurs
    while(1); // Fin de match
  }
  else {
    pami.disableMotors(); // Desactive les moteurs
  }  
}