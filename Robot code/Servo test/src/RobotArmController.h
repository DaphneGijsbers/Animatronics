#pragma once

#ifndef NO_ARDUINO
#include <Arduino.h>
#else
#include <string>
using String = std::string;
#endif

class RobotArmController {
public:
  RobotArmController();
  void voerActieUit(const String& actie);
};
