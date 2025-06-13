#ifndef NO_ARDUINO
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <string>
#else
#include <string>
#endif

#include "IUdpHandler.h"

class UdpHandler : public IUdpHandler {
public:
    UdpHandler(const char* ssid = nullptr, const char* password = nullptr,
               const char* laptopIP = nullptr, unsigned int laptopPort = 0, unsigned int localPort = 0);

    void begin() override;
    void sendMessage(const std::string& message) override;
    std::string receiveMessage() override;

private:
#ifndef NO_ARDUINO
    const char* ssid;
    const char* password;
    const char* laptopIP;
    unsigned int laptopPort;
    unsigned int localPort;
    WiFiUDP udp;
#endif
};
