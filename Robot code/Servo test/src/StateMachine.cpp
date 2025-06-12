#include "StateMachine.h"


StateMachine::StateMachine(UdpHandler& udp, RobotArmController& arm)
  : udpHandler(udp), robotArm(arm) {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  afstand = 0;  // Initialize mocked distance to 0
  currentState = State::IDLE;
  lastMovementTime = 0;
  laatsteCommando = "";
}

void StateMachine::update() {
  // Use mocked distance if set (non-zero), else read actual sensor distance
  long currentDistance = (afstand > 0) ? afstand : readDistanceCM();

  switch (currentState) {
    case State::IDLE:
      if (currentDistance > 0 && currentDistance < afstandDrempel) {
        Serial.println("📡 Beweging gedetecteerd → start camera");
        udpHandler.sendMessage("CMD_START_CAMERA");
        currentState = State::STREAMEN;
        lastMovementTime = millis();
      }
      break;

    case State::STREAMEN:
      Serial.println("⌛ Wachten op AI-reactie via WiFi...");
      currentState = State::WACHT_OP_AI;
      break;

    case State::WACHT_OP_AI:
      laatsteCommando = udpHandler.receiveMessage();
      if (laatsteCommando != "") {
        Serial.println("📨 AI-commando ontvangen: " + laatsteCommando);
        currentState = State::UITVOEREN;
      }
      break;

    case State::UITVOEREN:
      robotArm.voerActieUit(laatsteCommando);
      currentState = State::IDLE;
      break;
  }

  if (currentState != State::IDLE && millis() - lastMovementTime > timeout) {
    Serial.println("⏳ Timeout - terug naar IDLE");
    currentState = State::IDLE;
  }
}

long StateMachine::readDistanceCM() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH, 30000);
  return duration * 0.034 / 2;
}

// Testing helpers:

State StateMachine::getCurrentState() const {
  return currentState;
}

void StateMachine::mockAfstand(long mockAfstand) {
  afstand = mockAfstand;
}

void StateMachine::mockTimeElapsed(unsigned long elapsed) {
  // Simulate that "elapsed" milliseconds passed since lastMovementTime
  lastMovementTime -= elapsed;
}

void StateMachine::forceState(State state) {
  currentState = state;
}
