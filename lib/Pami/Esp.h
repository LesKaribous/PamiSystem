#pragma once

#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

#include <Ihm.h>
#include <Pami.h>

#include "pins.h" 

// Définition variables pour ESPNow
uint8_t PAMI_1[] = {0xF4, 0x12, 0xFA, 0x53, 0x6E, 0x60}; // Sender
uint8_t PAMI_2[] = {0xF4, 0x12, 0xFA, 0x4C, 0x07, 0x94}; // Receiver
uint8_t PAMI_3[] = {0xF4, 0x12, 0xFA, 0x53, 0x6D, 0x08}; // Receiver

typedef struct message_struct {
  int order;
} message_struct;

class Esp // TODO change to EspNow
{
private:
    message_struct message;
    esp_now_peer_info_t peerInfo;

    Robot::Pami *_robot;
public:
    Esp(/* args */);
    ~Esp();

    bool init(Robot::Pami &robot);
    void printMacAdress();
    bool BroadcastMessage(int orderMessage);
    
    static void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
    static void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len);
};

