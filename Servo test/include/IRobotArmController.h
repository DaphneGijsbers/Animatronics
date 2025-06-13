#ifndef IROBOT_ARM_CONTROLLER_H
#define IROBOT_ARM_CONTROLLER_H

#include <string>

class IRobotArmController {
public:
    virtual ~IRobotArmController() = default;

    virtual void voerActieUit(const std::string& actie) = 0;
};

#endif // IROBOT_ARM_CONTROLLER_H
