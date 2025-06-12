#include <unity.h>
#include <string>
#include "StateMachine.h"

// --- Mocks ---

class UdpHandlerMock {
public:
    UdpHandlerMock() {}

    void begin() {}
    void sendMessage(const std::string& message) {
        lastSentMessage = message;
    }
    std::string receiveMessage() {
        return mockedReceivedMessage;
    }
    void setMockedReceivedMessage(const std::string& msg) {
        mockedReceivedMessage = msg;
    }
    std::string getLastSentMessage() const {
        return lastSentMessage;
    }
private:
    std::string mockedReceivedMessage = "";
    std::string lastSentMessage = "";
};

class RobotArmControllerMock {
public:
    RobotArmControllerMock() : actionExecuted(false), lastAction("") {}
    void voerActieUit(const std::string& actie) {
        actionExecuted = true;
        lastAction = actie;
    }
    bool actionExecuted;
    std::string lastAction;
};

// --- Test set-up ---

UdpHandlerMock udpMock;
RobotArmControllerMock armMock;
StateMachine* sm;

void setUp() {
    sm = new StateMachine((UdpHandler&)udpMock, (RobotArmController&)armMock);
}

void tearDown() {
    delete sm;
}

// --- Test cases ---

void test_initial_state_idle() {
    TEST_ASSERT_EQUAL(static_cast<int>(State::IDLE), static_cast<int>(sm->getCurrentState()));
}

void test_transition_idle_to_streamen_and_send() {
    sm->mockAfstand(30);
    sm->update();
    TEST_ASSERT_EQUAL(static_cast<int>(State::STREAMEN), static_cast<int>(sm->getCurrentState()));
    TEST_ASSERT_EQUAL_STRING("CMD_START_CAMERA", udpMock.getLastSentMessage().c_str());
}

void test_streamen_to_wait_for_ai() {
    sm->mockAfstand(30);
    sm->update(); // STREAMEN
    sm->update(); // WACHT_OP_AI
    TEST_ASSERT_EQUAL(static_cast<int>(State::WACHT_OP_AI), static_cast<int>(sm->getCurrentState()));
}

void test_receive_ai_message_and_execute() {
    sm->forceState(State::WACHT_OP_AI);
    udpMock.setMockedReceivedMessage("zwaaien");
    sm->update(); // To UITVOEREN
    TEST_ASSERT_EQUAL(static_cast<int>(State::UITVOEREN), static_cast<int>(sm->getCurrentState()));

    sm->update(); // UITVOEREN → IDLE
    TEST_ASSERT_TRUE(armMock.actionExecuted);
    TEST_ASSERT_EQUAL_STRING("zwaaien", armMock.lastAction.c_str());
    TEST_ASSERT_EQUAL(static_cast<int>(State::IDLE), static_cast<int>(sm->getCurrentState()));
}

void test_timeout_resets_to_idle() {
    sm->forceState(State::STREAMEN);
    sm->mockTimeElapsed(20000); // 20 sec timeout
    sm->update();
    TEST_ASSERT_EQUAL(static_cast<int>(State::IDLE), static_cast<int>(sm->getCurrentState()));
}

// --- Main ---

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_initial_state_idle);
    RUN_TEST(test_transition_idle_to_streamen_and_send);
    RUN_TEST(test_streamen_to_wait_for_ai);
    RUN_TEST(test_receive_ai_message_and_execute);
    RUN_TEST(test_timeout_resets_to_idle);
    return UNITY_END();
}
