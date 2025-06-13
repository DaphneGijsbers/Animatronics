#pragma once

#include "IRobotArmController.h"
#include <string>

#ifndef NO_ARDUINO
#include <Arduino.h>
#endif

class RobotArmController : public IRobotArmController {
public:
    RobotArmController();

    void voerActieUit(const std::string& actie) override;

private:
    // Voeg hier eventueel private members toe zoals servo objecten e.d.
};
