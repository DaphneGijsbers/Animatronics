#include "UdpHandler.h"

#ifndef NO_ARDUINO
#include <Arduino.h>
#endif

UdpHandler::UdpHandler(const char* ssid, const char* password, const char* laptopIP, unsigned int laptopPort, unsigned int localPort)
  : ssid(ssid), password(password), laptopIP(laptopIP), laptopPort(laptopPort), localPort(localPort) {}

void UdpHandler::begin() {
#ifndef NO_ARDUINO
  WiFi.begin(ssid, password);
  Serial.print("🔌 Verbinden met WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Verbonden!");
  Serial.print("IP-adres: ");
  Serial.println(WiFi.localIP());
  udp.begin(localPort);
#else
  // Native build: geen WiFi, dummy implementatie
#endif
}

void UdpHandler::sendMessage(const std::string& message) {
#ifndef NO_ARDUINO
  String msgArduino = String(message.c_str());
  udp.beginPacket(laptopIP, laptopPort);
  udp.print(msgArduino);
  udp.endPacket();
#else
  // Native build: dummy of logging (optioneel)
#endif
}

std::string UdpHandler::receiveMessage() {
#ifndef NO_ARDUINO
  int packetSize = udp.parsePacket();
  if (packetSize) {
    char incoming[255];
    int len = udp.read(incoming, 255);
    if (len > 0) {
      incoming[len] = 0;
      String bericht = String(incoming);
      bericht.trim();
      return std::string(bericht.c_str());
    }
  }
  return "";
#else
  // Native build: dummy return lege string
  return "";
#endif
}
