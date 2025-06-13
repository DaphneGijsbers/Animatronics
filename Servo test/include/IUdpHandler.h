#ifndef IUDP_HANDLER_H
#define IUDP_HANDLER_H

#include <string>

class IUdpHandler {
public:
    virtual ~IUdpHandler() = default;

    virtual void begin() = 0;
    virtual void sendMessage(const std::string& message) = 0;
    virtual std::string receiveMessage() = 0;
};

#endif // IUDP_HANDLER_H
