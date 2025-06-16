#include "RobotArmController.h"

#ifndef NO_ARDUINO
#include <Arduino.h>
#endif

RobotArmController::RobotArmController(Servo& elbow1, Servo& elbow2, Servo& thumb)
    : _elbow1(elbow1), _elbow2(elbow2), _thumb(thumb) {
    
#ifndef NO_ARDUINO
    
#endif
}

void RobotArmController::voerActieUit(const std::string& actie) {
#ifndef NO_ARDUINO
    // Converteer std::string naar Arduino String
    String actieArduino = String(actie.c_str());

    
    if (actieArduino == "zwaaien") {
       for (int pos = 0; pos <= 90; pos += 5) { // naar 90 graden
            _elbow1.write(pos);
            delay(20);
        }
        for (int pos = 90; pos >= 0; pos -= 5) { // terug naar 0 graden
            _elbow1.write(pos);
            delay(20);
        }
    } 
    else if (actieArduino == "open") {
        // Voorbeeld: hand openen (bijv. thumb servo)
        for (int pos = 90; pos >= 0; pos -= 5) { 
            _thumb.write(pos);
            delay(20);
        }
    }
    else if (actieArduino == "dicht"){
        for (int pos = 0; pos <= 90; pos += 5) { 
            _thumb.write(pos);
            delay(20);
        }
    }
    
#else
    // Native build: dummy implementatie, kan ook logging doen
#endif
}
