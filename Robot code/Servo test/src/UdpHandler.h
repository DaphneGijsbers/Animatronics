#pragma once

#ifndef NO_ARDUINO
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#else
#include <string>
using String = std::string;
#endif

class UdpHandler {
public:
    // Constructor parameters kunnen dummy zijn in native test
    UdpHandler(const char* ssid = nullptr, const char* password = nullptr,
               const char* laptopIP = nullptr, unsigned int laptopPort = 0, unsigned int localPort = 0);

    void begin();
    void sendMessage(const String& message);
    String receiveMessage();

private:
#ifndef NO_ARDUINO
    const char* ssid;
    const char* password;
    const char* laptopIP;
    unsigned int laptopPort;
    unsigned int localPort;
    WiFiUDP udp;
#else
    // Native test mocks kunnen hier lege of dummy data hebben
#endif
};
