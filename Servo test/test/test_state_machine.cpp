#include <unity.h>
#include <string>


#include "UdpHandlerMock.h"
#include "RobotArmControllerMock.h"

#include "StateMachine.h"

static UdpHandlerMock udpMock;
static RobotArmControllerMock robotMock;
static StateMachine* sm;

void setUp(void) {
    // Maak StateMachine aan met mocks
    sm = new StateMachine(udpMock, robotMock);
}

void tearDown(void) {
    delete sm;
}

void test_initial_state(void) {
    // Controleer dat er niets onverwachts gebeurt in de beginfase
    TEST_ASSERT_EQUAL(false, robotMock.actionExecuted);
    TEST_ASSERT_EQUAL_STRING("", robotMock.lastAction.c_str());
    TEST_ASSERT_EQUAL_STRING("", udpMock.getLastSentMessage().c_str());
}

void test_send_receive_message(void) {
    
    udpMock.setMockedReceivedMessage("Hello");

    sm->update();

    // Test of de mock ontvangen bericht teruggeeft zoals verwacht
    std::string received = udpMock.receiveMessage();
    TEST_ASSERT_EQUAL_STRING("Hello", received.c_str());

}

int main(int argc, char **argv) {
    (void)argc; (void)argv;
    UNITY_BEGIN();

    RUN_TEST(test_initial_state);
    RUN_TEST(test_send_receive_message);

    return UNITY_END();
}
