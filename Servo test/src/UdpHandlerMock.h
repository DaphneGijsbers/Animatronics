#pragma once
#include "IUdpHandler.h"
#include <string>

class UdpHandlerMock : public IUdpHandler {
public:
    UdpHandlerMock();
    virtual ~UdpHandlerMock() = default;

    void begin() override;
    void sendMessage(const std::string& message) override;
    std::string receiveMessage() override;

    void setMockedReceivedMessage(const std::string& msg);
    std::string getLastSentMessage() const;

private:
    std::string lastMessage;      
    std::string mockedMessage;    
};
