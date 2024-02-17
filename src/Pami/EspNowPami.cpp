#include "EspNowPami.h"
#include "Ihm/Ihm.h"
#include "Pami.h"

namespace Com{

EspNowPami::EspNowPami(/* args */)
{

}

EspNowPami::~EspNowPami()
{
}

bool EspNowPami::init(){
  bool initState = false;
  bool addPeerState = true;
  bool messageState = true;
  WiFi.mode(WIFI_STA);

  // Init EspNow-NOW
  if (esp_now_init() != ESP_OK) 
  {
    Ihm::getInstance().debug("Error init EspNow-NOW !");
    initState = false;
  } else {initState = true;}
  // Si Init OK
  if(initState == true)
  {
    if(Robot::robot->getID() == 1) //Si Robot Principal
    {
      esp_now_register_send_cb(OnDataSent);
      // register peer
      peerInfo.channel = 0;
      peerInfo.encrypt = false;
      // register first peer
      memcpy(peerInfo.peer_addr, PAMI_2, 6);
      if (esp_now_add_peer(&peerInfo) != ESP_OK)
      {
        Serial.println("Failed to add peer");
        addPeerState = false;
      }else{addPeerState = true;}
      // register second peer
      memcpy(peerInfo.peer_addr, PAMI_3, 6);
      if (esp_now_add_peer(&peerInfo) != ESP_OK)
      {
        Serial.println("Failed to add peer");
        addPeerState = false;
      }else{addPeerState = true;}
    } else {esp_now_register_recv_cb(OnDataRecv);}// get recv packer info
  }
  if(initState && addPeerState && Robot::robot->getID() == 1)
  {
    messageState = BroadcastMessage(static_cast<int>(Robot::State::PAIRING));
    if (messageState) Robot::robot->setState(Robot::State::PAIRED);
  }
  return initState && addPeerState && messageState;
}

void EspNowPami::printMacAdress(){
  Serial.println(WiFi.macAddress());
}

bool EspNowPami::BroadcastMessage(int orderMessage){

  bool sendState = true;
  message.order = orderMessage;

  if(Robot::robot->getID() == 1)
  {
    esp_err_t resultPami2 = esp_now_send(PAMI_2, (uint8_t *) &message, sizeof(message));
    esp_err_t resultPami3 = esp_now_send(PAMI_3, (uint8_t *) &message, sizeof(message));

    if (resultPami2 != ESP_OK || resultPami3 != ESP_OK ){
      if (resultPami2 != ESP_OK) Serial.println("Error sending the data to PAMI2");
      if (resultPami3 != ESP_OK) Serial.println("Error sending the data to PAMI3");
      sendState = false;
    }
    else
    {
      Serial.println("Sent with success");
      sendState = true;
    }
  }
  return sendState;
}

void EspNowPami::OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  Serial.print("Packet to: ");
  // Copies the sender mac address to a string
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
  Serial.print(" send status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void EspNowPami::OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  message_struct messageRecv;
  memcpy(&messageRecv, incomingData, sizeof(messageRecv));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("order: ");
  Serial.println(messageRecv.order);
  if (messageRecv.order == static_cast<int>(Robot::State::PAIRING)) Robot::robot->setState(Robot::State::PAIRED);
  if (messageRecv.order == static_cast<int>(Robot::State::ARMED)) Robot::robot->setState(Robot::State::READY);
  if (messageRecv.order == static_cast<int>(Robot::State::MATCH_STARTED)) Robot::robot->setState(Robot::State::MATCH_STARTED);
  Serial.println(static_cast<int>(Robot::robot->getState()));
  Serial.println();
}

}