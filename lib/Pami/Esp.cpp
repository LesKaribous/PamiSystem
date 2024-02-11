#include "Esp.h"

Esp::Esp(/* args */)
{

}

Esp::~Esp()
{
}

bool Esp::init(Robot::Pami &robot){
  _robot = &robot;

  bool initState = false;
  bool addPeerState = true;
  bool messageState = true;
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) 
  {
    Ihm::getInstance().debug("Error init ESP-NOW !");
    initState = false;
  } else {initState = true;}
  // Si Init OK
  if(initState == true)
  {
    if(_robot->getID() == 1) //Si Robot Principal
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
  if(initState && addPeerState && _robot->getID() == 1)
  {
    messageState = BroadcastMessage(Robot::State::PAIRING);
    if (messageState) _robot->setState(Robot::State::PAIRED);
  }
  return initState && addPeerState && messageState;
}

void Esp::printMacAdress(){
  Serial.println(WiFi.macAddress());
}

bool Esp::BroadcastMessage(int orderMessage){

  bool sendState = true;
  message.order = orderMessage;

  if(_robot->getID() == 1)
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

void Esp::OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  Serial.print("Packet to: ");
  // Copies the sender mac address to a string
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
  Serial.print(" send status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void Esp::OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  message_struct messageRecv;
  memcpy(&messageRecv, incomingData, sizeof(messageRecv));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("order: ");
  Serial.println(messageRecv.order);
  if (messageRecv.order == Robot::State::PAIRING) robotState = PAIRED;
  if (messageRecv.order == Robot::State::ARMED) robotState = READY;
  if (messageRecv.order == Robot::State::MATCH_STARTED) robotState = MATCH_STARTED;
  Serial.println(robotState);
  Serial.println();
}


