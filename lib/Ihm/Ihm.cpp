#include "Ihm.h"

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R2);

Ihm::Ihm(){
    led = Adafruit_NeoPixel(1, Pin::ledStatus, NEO_GRB + NEO_KHZ800);
}

Ihm &Ihm::getInstance(){
    static Ihm ihm_;
    return ihm_;
}

void Ihm::init(){
    // Init pins
    pinMode(Pin::ColorTeam,INPUT_PULLUP);

    // Init functions
    Wire.begin();
    Serial.begin(115200);
    initLedStatus();
    initLCD();
    // Debug
    debug("PAMI Started");
}

void Ihm::initLCD(){
  u8g2.begin();
  u8g2.clearBuffer(); // clear the internal memory
}

Robot::Pami& Ihm::setRobot(Robot::Pami &robot){
  if(_robot != nullptr)
    _robot = &robot;

  return *_robot;
}

void Ihm::pairingScreen(){
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_streamline_interface_essential_wifi_t);
  u8g2.drawGlyphX2(0,42,0x0032); // Draw signal ESPNow
  // Mettre à jour l'écran
  u8g2.sendBuffer();
  delay(800);
  // Draw Text
  u8g2.setFont(u8g2_font_t0_22b_mf);
  u8g2.drawStr(49, 13, "ESPNow");
  u8g2.setFont(u8g2_font_5x7_mf);
  u8g2.drawStr(50, 21, "Pairing ...");
  u8g2.setFont(u8g2_font_tiny5_tf);
  u8g2.drawStr(50, 30, "Plug tirette to ignore");
  // Mettre à jour l'écran
  u8g2.sendBuffer();
  // Wait pairing or tirette
  while(!_robot->getTirette() && !initEspNow())
  {
    delay(250);
  }
  printMacAdress();
  u8g2.clearBuffer();
}

void Ihm::drawSplashScreen(){
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_streamline_coding_apps_websites_t);
  u8g2.drawGlyphX2(0,42,0x0043); // Draw LadyBug
  // Mettre à jour l'écran
  u8g2.sendBuffer();
  delay(800);
  // Draw Text
  u8g2.setFont(u8g2_font_t0_22b_mf);
  u8g2.drawStr(49, 13, "PAMI 24");
  u8g2.setFont(u8g2_font_5x7_mf);
  u8g2.drawStr(50, 21, "Les Karibous");
  // Créer une String avec la date et l'heure de compilation
  u8g2.setFont(u8g2_font_tiny5_tf);
  String compileDateTime = String(__DATE__) + " " + String(__TIME__);
  u8g2.drawStr(50, 30, compileDateTime.c_str());
  // Mettre à jour l'écran
  u8g2.sendBuffer();
  delay(2000);
  u8g2.clearBuffer();
}

void Ihm::drawBackLcd(){
  //--------------------------------------------------------
  // Draw Bot Number
  String stringValue = String(_robot->getID());  // Convertir le byte en String
  const char* result = stringValue.c_str();       // Convertir la String en const char*
  u8g2.setFont(u8g2_font_5x7_mf);
  u8g2.drawStr(0, 13, "bot");
  u8g2.setFont(u8g2_font_t0_22b_mf);
  u8g2.drawStr(14, 13, result);
  // Draw separators
  u8g2.drawLine(0,18,128,18);
  u8g2.drawLine(50,15,50,0);
  // Debug texte
  u8g2.setFont(u8g2_font_5x7_mf); // Mini font for debug - 6 heigh monospace
  u8g2.drawStr(0, 31, "debug: ");
  // Mettre à jour l'écran
  u8g2.sendBuffer();
}

Robot::Team Ihm::checkColorTeam(){
  bool temp = digitalRead(Pin::ColorTeam);
  int team = (Robot::Team)temp;
  
  if(_robot->getTeam() != team || _robot->getTeam() == Robot::Team::UNDEFINED){
    _robot->setTeam(Robot::Team(team));
    led.setBrightness(50);

    if(_robot->getTeam() == Robot::Team::BLUE){
      led.setPixelColor(0,led.Color(0,0,255)); // LED en BLEU
      Ihm::getInstance().debug("Team Blue");
    }
    else {
      led.setPixelColor(0,led.Color(255,255,0)); // LED en JAUNE
       Ihm::getInstance().debug("Team Yellow");
    }
    led.show();
  }

  return _robot->getTeam();
}