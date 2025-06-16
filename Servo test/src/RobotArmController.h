#pragma once

#include "IRobotArmController.h"
#include <string>

#ifndef NO_ARDUINO
#include <Arduino.h>
#include <ESP32Servo.h>
#endif

class RobotArmController : public IRobotArmController {
public:
    RobotArmController(Servo& elbow1, Servo& elbow2, Servo& thumb);

    void voerActieUit(const std::string& actie) override;

private:
    Servo& _elbow1;
    Servo& _elbow2;
    Servo& _thumb;
};
