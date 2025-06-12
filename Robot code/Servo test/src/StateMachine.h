#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#ifdef NO_ARDUINO
  #include <string>
  #include <cstdint>
  
  // Gebruik std::string als String
  using String = std::string;

  // typedefs zoals uint32_t uit <cstdint> gebruiken, geen eigen definities!
#else
  #include <Arduino.h>
#endif

#include "UdpHandler.h"
#include "RobotArmController.h"

enum class State {
  IDLE,
  STREAMEN,
  WACHT_OP_AI,
  UITVOEREN
};

class StateMachine {
public:
  StateMachine(UdpHandler& udp, RobotArmController& arm);

  void update();

  // Testing helpers:
  State getCurrentState() const;
  void mockAfstand(long afstand);
  void mockTimeElapsed(uint32_t elapsed);
  void forceState(State state);

private:
  UdpHandler& udpHandler;
  RobotArmController& robotArm;

  State currentState = State::IDLE;
  uint32_t lastMovementTime = 0;
  String laatsteCommando = "";
  
  // Pins for sensor (gebruik alleen als Arduino actief is)
#ifndef NO_ARDUINO
  const int trigPin = 5;  
  const int echoPin = 18; 
#endif

  const long afstandDrempel = 40; // distance threshold in cm
  const uint32_t timeout = 15000; // 15 seconds timeout
  
  long afstand = 0; // store mocked or real distance for tests

  long readDistanceCM();
};

#endif // STATEMACHINE_H
