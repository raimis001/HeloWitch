#include "arduino.h"
#include <Servo.h>

class ServoController
{
  public:
    int MOVING_TIME = 10;

    int angle_start = 0; // 0°
    int angle_stop = 90; // 90°

    ServoController(int servoPin)
    {
      servo.attach(servoPin);
      servo.write(angle_stop * servoConst);
      
      delay(1000);
    }

    ~ServoController();

    void update(int dir)
    {
      if (lastDir != dir) 
      {
        last_time = millis();
        deltaTime = 0;
        lastDir = dir;
        //Serial.println("New start");
      }

      
      deltaTime += (millis() - last_time);
      last_time = millis();

      if (deltaTime > MOVING_TIME * 1000) 
      {
        return;
      }

      float progress = (float)deltaTime / (float)(MOVING_TIME * 1000);
      
      long angle = 0; 
      if (dir == 1) {
        angle = map(deltaTime, 0, MOVING_TIME * 1000, angle_stop * servoConst, angle_start * servoConst);
      } else {
        angle = map(deltaTime, 0, MOVING_TIME * 1000, angle_start * servoConst, angle_stop * servoConst);
      }
      //Serial.print(angle);Serial.print(" ");
      servo.write(angle);
    }
  private:
    Servo servo;
    double servoConst = 90.00 / 90.00;
    int lastDir = -1;
    unsigned long last_time = 0;
    unsigned long deltaTime = 0;
};