#include "StateMachine.h"

#ifndef NO_ARDUINO
#include <Arduino.h>
#endif

StateMachine::StateMachine(IUdpHandler& udp, IRobotArmController& arm) 
  : udpHandler(udp), robotArm(arm), currentState(State::IDLE), lastMovementTime(0), afstand(0) { }

void StateMachine::update() {
#ifndef NO_ARDUINO
  afstand = readDistanceCM();
#else
  // afstand kan gemockt worden
#endif

  uint32_t now;
#ifndef NO_ARDUINO
  now = millis();
#else
  now = lastMovementTime;  // Gebruik lastMovementTime als 'tijd' in tests, of implementeer eigen timer
#endif

  if (currentState != State::IDLE && now - lastMovementTime > timeout) {
    currentState = State::IDLE;
  }

  switch(currentState) {
    case State::IDLE:
      if (afstand > 0 && afstand < afstandDrempel) {
        currentState = State::STREAMEN;
        lastMovementTime = now;
        udpHandler.sendMessage("CMD_START_CAMERA");
      }
      break;

    case State::STREAMEN:
      currentState = State::WACHT_OP_AI;
      break;

    case State::WACHT_OP_AI: {
      std::string cmd = udpHandler.receiveMessage();
      if (!cmd.empty()) {
        laatsteCommando = cmd;
        currentState = State::UITVOEREN;
      }
      break;
    }

    case State::UITVOEREN:
      robotArm.voerActieUit(laatsteCommando);
      currentState = State::IDLE;
      break;
  }
}

State StateMachine::getCurrentState() const {
  return currentState;
}

void StateMachine::mockAfstand(long a) {
  afstand = a;
}

void StateMachine::mockTimeElapsed(uint32_t elapsed) {
  // In test kun je hier lastMovementTime aanpassen om 'verlopen tijd' te simuleren
  lastMovementTime = elapsed;
}

void StateMachine::forceState(State state) {
  currentState = state;
}

#ifndef NO_ARDUINO
long StateMachine::readDistanceCM() {
  // Sensor code, bijvoorbeeld HC-SR04 afstandsensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  long distance = duration * 0.034 / 2;
  return distance;
}
#else
long StateMachine::readDistanceCM() {
  return afstand; // return gemockte waarde tijdens testen native
}
#endif
