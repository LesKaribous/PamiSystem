#pragma once

#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

#include "pins.h" 

// Définition variables pour EspNowPami
const uint8_t PAMI_1[] = {0xF4, 0x12, 0xFA, 0x53, 0x6E, 0x60}; // Sender
const uint8_t PAMI_2[] = {0xF4, 0x12, 0xFA, 0x4C, 0x07, 0x94}; // Receiver
const uint8_t PAMI_3[] = {0xF4, 0x12, 0xFA, 0x53, 0x6D, 0x08}; // Receiver

namespace Com
{

typedef struct message_struct {
  int order;
} message_struct;

class EspNowPami 
{
private:
    message_struct message;
    esp_now_peer_info_t peerInfo;
public:
    EspNowPami(/* args */);
    ~EspNowPami();

    bool init();
    void printMacAdress();
    bool BroadcastMessage(int orderMessage);
    
    static void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
    static void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len);
};

  
} // namespace Com