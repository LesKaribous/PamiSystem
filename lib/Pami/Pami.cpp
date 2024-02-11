#include "Pami.h"
namespace Robot{

Pami::Pami(/* args */)
{

}

Pami::~Pami()
{
    motor_D = std::make_unique<Actuator::StepperMotor>(Pin::EN, Pin::DIR_D, Pin::STEP_D, Pin::MS1, Pin::MS2);
    motor_G = std::make_unique<Actuator::StepperMotor>(Pin::EN, Pin::DIR_G, Pin::STEP_G, Pin::MS1, Pin::MS2);
}

void Pami::init(){
    pinMode(Pin::Tirette,INPUT_PULLUP);
    pinMode(Pin::BotNumb01,INPUT_PULLUP);
    pinMode(Pin::BotNumb02,INPUT_PULLUP);

    readRobotID();

    for(size_t i = 0; i < 2; i++){
        distance_sensors[i].setShutPin(Pin::xshutPins[i]);
        distance_sensors[i].init(i);
    }

    motor_D->init();
    motor_G->init();

    antenna.init(Pin::pinServo);

    enableMotors();
    antenna.up();
}

void Pami::readRobotID(){
  bool bit1 = !digitalRead(Pin::BotNumb01);
  bool bit2 = !digitalRead(Pin::BotNumb02);
  _id = (bit2 << 1) | bit1;
}

void Pami::enableMotors(){
    motor_D->enable();
    motor_G->enable();
}

void Pami::disableMotors(){
    motor_D->disable();
    motor_G->disable();
}


void Pami::go(float _dist){
  long stepValue = convertDistToStep(_dist);
  motor_G->move(stepValue);
  motor_D->move(-stepValue);
  processMove();
}

void Pami::turn(float _angle){
  long stepValue = convertAngleToStep(_angle);
  motor_G->move(stepValue);
  motor_D->move(stepValue);
  processMove();
}

void Pami::updateMotors(){
  motor_D->update();
  motor_G->update();
}

void Pami::processMove(){
  // WARNING : Blocking function
  // WIP
  Ihm::getInstance().debug("Processing Move...");
  while(motor_D->isRunning() || motor_G->isRunning()){
    updateMotors();
  }
  Ihm::getInstance().debug("Movement ok");
}

long Pami::convertDistToStep(float _dist) {
  float revolutions = _dist / _circumferenceMM;
  return static_cast<long>(revolutions * STEPS_PER_REVOLUTION * motor_D->getStepMulitplier());
}

// Convert angle to step for one wheel (robot turning on center)
long Pami::convertAngleToStep(float _angle) {
    float angleRadians = _angle * (PI / 180.0f); // Conversion en radians
    float arcLength = (WHEEL_DISTANCE_MM / 2.0f) * angleRadians; // Longueur de l'arc parcouru par chaque roue
    float revolutions = arcLength / _circumferenceMM;
    return static_cast<long>(revolutions * STEPS_PER_REVOLUTION * motor_D->getStepMulitplier());
}

void Pami::convertToPolar(Pose _target){
  convertToPolar(_target.x, _target.y, _target.rot);
}

void Pami::convertToPolar(float _x, float _y){
  float dx = _x - currentPose.x;
  float dy = _y - currentPose.y;

  float targetAngleRadians = atan2(dy, dx);
  float currentRotRadians = currentPose.rot * (M_PI / 180.0f);

  targetMove.distance = sqrt(dx*dx + dy*dy);

  // Calculer la rotation la plus courte pour rotation1
  targetMove.rotation1 = (targetAngleRadians - currentRotRadians) * (180.0f / M_PI);
  if (targetMove.rotation1 > 180.0f) targetMove.rotation1 -= 360.0f;
  if (targetMove.rotation1 < -180.0f) targetMove.rotation1 += 360.0f;

  targetMove.rotation2 = 0; // Pas de rotation finale

  newPolarTarget = true;
}

void Pami::convertToPolar(float _x, float _y, float _rot){
  float dx = _x - currentPose.x;
  float dy = _y - currentPose.y;

  float targetAngleRadians = atan2(dy, dx);
  float currentRotRadians = currentPose.rot * (M_PI / 180.0f);
  float targetRotRadians = _rot * (M_PI / 180.0f);

  targetMove.distance = sqrt(dx*dx + dy*dy);

  // Calculer la rotation la plus courte pour rotation1
  targetMove.rotation1 = (targetAngleRadians - currentRotRadians) * (180.0f / M_PI);
  if (targetMove.rotation1 > 180.0f) targetMove.rotation1 -= 360.0f;
  if (targetMove.rotation1 < -180.0f) targetMove.rotation1 += 360.0f;

  // Calculer la rotation la plus courte pour rotation2
  targetMove.rotation2 = (targetRotRadians - targetAngleRadians) * (180.0f / M_PI);
  if (targetMove.rotation2 > 180.0f) targetMove.rotation2 -= 360.0f;
  if (targetMove.rotation2 < -180.0f) targetMove.rotation2 += 360.0f;

  newPolarTarget = true;
}

}