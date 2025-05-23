#include <Servo.h>

Servo servo;
Servo servo2;
int pos = 0;

void setup() 
{
    Serial.begin(9600);
    servo.attach(9);
    servo2.attach(8);
}

void loop() 
{
    for(pos = 0; pos <= 180; pos += 1)
    {
      servo.write(pos);
      servo2.write(pos);
      Serial.println(pos);
      delay(10);
    }
    for(pos = 180; pos >= 0; pos -= 1)
    {
      servo.write(pos);
      servo2.write(pos);
      Serial.println(pos);
      delay(10);
    }
}
