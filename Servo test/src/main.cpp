#include <Arduino.h>
#include "UdpHandler.h"
#include "RobotArmController.h"
#include "StateMachine.h"

const char* ssid = "Appartement Dirk";
const char* password = "Stijnwoonthier!23";
const char* laptopIP = "192.168.178.163";
const unsigned int laptopPort = 4210;
const unsigned int localPort = 4211;

UdpHandler udp(ssid, password, laptopIP, laptopPort, localPort);
RobotArmController robotArm;
StateMachine stateMachine(udp, robotArm);

void setup() {
  Serial.begin(115200);
  udp.begin();
}

void loop() {
  // Ontvang bericht
  String message = udp.receiveMessage();
  if (message.length() > 0) {
    Serial.println("Ontvangen: " + message);

    if (message == "WAVE") {
      Serial.println("Zwaaicommand ontvangen - robot zwaait!");
      robotArm.voerActieUit("zwaaien");  // Of een eigen functie
    }
  }

  stateMachine.update();
  delay(10);
}
