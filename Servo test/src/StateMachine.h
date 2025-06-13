#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#ifdef NO_ARDUINO
  #include <string>
  #include <cstdint>
  using String = std::string;
#else
  #include <Arduino.h>
#endif

#include "IUdpHandler.h"
#include "IRobotArmController.h"

enum class State {
  IDLE,
  STREAMEN,
  WACHT_OP_AI,
  UITVOEREN
};

class StateMachine {
public:
  StateMachine(IUdpHandler& udp, IRobotArmController& arm);

  void update();

  // Testing helpers:
  State getCurrentState() const;
  void mockAfstand(long afstand);
  void mockTimeElapsed(uint32_t elapsed);
  void forceState(State state);

private:
  IUdpHandler& udpHandler;
  IRobotArmController& robotArm;

  State currentState = State::IDLE;
  uint32_t lastMovementTime = 0;
  String laatsteCommando = "";

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
