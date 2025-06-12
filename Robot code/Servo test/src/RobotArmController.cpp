#include "RobotArmController.h"

RobotArmController::RobotArmController() {}

void RobotArmController::voerActieUit(const String& actie) {
  if (actie == "zwaaien") {
    Serial.println("🤖 Robotarm zwaait!");
    // Voeg servo/motor code toe
  } else if (actie == "rechts") {
    Serial.println("🤖 Robotarm naar rechts!");
    // Voeg servo/motor code toe
  } else if (actie == "links") {
    Serial.println("🤖 Robotarm naar links!");
    // Voeg servo/motor code toe
  } else {
    Serial.println("⚠️ Onbekend commando: " + actie);
  }
}


