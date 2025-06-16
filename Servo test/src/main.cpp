#ifndef NO_ARDUINO
#include <Arduino.h>
#include <ESP32Servo.h>
#include "UdpHandler.h"
#include "RobotArmController.h"

Servo elbow1;
Servo elbow2;
Servo thumb;
int pos = 0;

#endif
#include "StateMachine.h"

const char* ssid = "Appartement Dirk";
const char* password = "Stijnwoonthier!23";
const char* laptopIP = "192.168.178.163";
const unsigned int laptopPort = 4210;
const unsigned int localPort = 4211;

UdpHandler udp(ssid, password, laptopIP, laptopPort, localPort);
RobotArmController* robotArm = nullptr;
StateMachine* stateMachine = nullptr;
//RobotArmController robotArm;
//StateMachine stateMachine(udp, robotArm);

void setup() {
  Serial.begin(115200);
  udp.begin();

#ifndef NO_ARDUINO
  elbow1.attach(25);
  elbow2.attach(26);
  thumb.attach(27);
#endif
  robotArm = new RobotArmController(elbow1, elbow2, thumb);
  stateMachine = new StateMachine(udp, *robotArm);
}

void loop() {
  if (robotArm == nullptr) {
    Serial.println("Error: robotArm is nullptr!");
    delay(1000);
    return;
  }
  // Ontvang bericht
  String message = String(udp.receiveMessage().c_str());
  if (message.length() > 0) {
    Serial.println("Ontvangen: " + message);

    if (message == "WAVE") {
      Serial.println("Zwaaicommand ontvangen - robot zwaait!");
      robotArm->voerActieUit("zwaaien"); 
    }
    else if(message == "DICHT"){
      Serial.println("dicht ontvangen - duim omlaag");
      robotArm->voerActieUit("dicht");
    }
    else if(message == "OPEN"){
      Serial.println("open ontvangen - duim omhoog");
      robotArm->voerActieUit("open");
    }
  }

  stateMachine->update();
  delay(10);
}
