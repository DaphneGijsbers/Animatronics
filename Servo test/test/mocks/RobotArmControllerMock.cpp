#include "RobotArmControllerMock.h"

RobotArmControllerMock::RobotArmControllerMock() 
    : actionExecuted(false), lastAction("") {}

void RobotArmControllerMock::voerActieUit(const std::string& actie) {
    actionExecuted = true;
    lastAction = actie;
}
