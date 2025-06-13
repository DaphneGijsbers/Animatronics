#include "UdpHandlerMock.h"

UdpHandlerMock::UdpHandlerMock() {}

void UdpHandlerMock::begin() {}

void UdpHandlerMock::sendMessage(const std::string& message) {
    lastMessage = message;
}

std::string UdpHandlerMock::receiveMessage() {
    return mockedMessage;
}

void UdpHandlerMock::setMockedReceivedMessage(const std::string& msg) {
    mockedMessage = msg;
}

std::string UdpHandlerMock::getLastSentMessage() const {
    return lastMessage;
}
