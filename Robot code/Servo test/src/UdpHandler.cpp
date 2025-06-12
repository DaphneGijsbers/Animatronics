#include "UdpHandler.h"

UdpHandler::UdpHandler(const char* ssid, const char* password, const char* laptopIP, unsigned int laptopPort, unsigned int localPort)
  : ssid(ssid), password(password), laptopIP(laptopIP), laptopPort(laptopPort), localPort(localPort) {}

void UdpHandler::begin() {
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
}

void UdpHandler::sendMessage(const String& message) {
  udp.beginPacket(laptopIP, laptopPort);
  udp.print(message);
  udp.endPacket();
}

String UdpHandler::receiveMessage() {
  int packetSize = udp.parsePacket();
  if (packetSize) {
    char incoming[255];
    int len = udp.read(incoming, 255);
    if (len > 0) {
      incoming[len] = 0;
      String bericht = String(incoming);
      bericht.trim();
      return bericht;
    }
  }
  return "";
}
