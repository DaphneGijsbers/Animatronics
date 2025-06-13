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
    // Stel in wat de mock moet "ontvangen"
    udpMock.setMockedReceivedMessage("Hello");

    // Update StateMachine, die zou receiveMessage kunnen oproepen
    sm->update();

    // Test of de mock ontvangen bericht teruggeeft zoals verwacht
    std::string received = udpMock.receiveMessage();
    TEST_ASSERT_EQUAL_STRING("Hello", received.c_str());

    // Hier zou je ook checken of een actie is uitgevoerd, afhankelijk van je StateMachine-logic
    // Bijvoorbeeld:
    // TEST_ASSERT_TRUE(robotMock.actionExecuted);
    // TEST_ASSERT_EQUAL_STRING("someAction", robotMock.lastAction.c_str());

    // En als je verwacht dat er iets via UDP is gestuurd
    // TEST_ASSERT_EQUAL_STRING("expectedMessage", udpMock.getLastSentMessage().c_str());
}

int main(int argc, char **argv) {
    (void)argc; (void)argv;
    UNITY_BEGIN();

    RUN_TEST(test_initial_state);
    RUN_TEST(test_send_receive_message);

    return UNITY_END();
}
