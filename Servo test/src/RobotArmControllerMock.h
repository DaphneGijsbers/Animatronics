#ifndef ROBOTARMCONTROLLERMOCK_H
#define ROBOTARMCONTROLLERMOCK_H

#include <string>
#include "IRobotArmController.h"

class RobotArmControllerMock : public IRobotArmController {
public:
    RobotArmControllerMock();
    ~RobotArmControllerMock() override = default;

    void voerActieUit(const std::string& actie) override;

    bool actionExecuted;
    std::string lastAction;
};

#endif 
