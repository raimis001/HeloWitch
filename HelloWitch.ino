#include "fade.h"
#include "MP3Player.h"
#include "BlinkLed.h"
#include "ServoController.h"

//Pins

#define LANTERN_PIN 3
#define TEST_PIN 4
#define EYES_PIN 5

#define MOTION_SENSOR_PIN  7


#define BTN_PLAY 8

#define MP3_RX 10
#define MP3_TX 11
#define SERVO_PIN 9

// seconds
int MOVING_TIME = 15;
int SLEEPING_TIME = 30; 
int SOUND_TIME = 2;

unsigned long move_start_ms;
unsigned long blink_start_ms;

int state = 0; 

//Classes
FadeLED* eyesFade;
FadeLED* lanternFade;
BlinkLED* testBlink;
ServoController* servo;

MP3Player* mp3Plyer;

void setup() {
  //Serial.begin(9600);  

  eyesFade = new FadeLED(EYES_PIN,0.5);
  lanternFade = new FadeLED(LANTERN_PIN,2.0);
  testBlink = new BlinkLED(TEST_PIN, 3000, 200);
  servo = new ServoController(SERVO_PIN);

  pinMode(BTN_PLAY, INPUT);
  pinMode(TEST_PIN, OUTPUT);
  pinMode(MOTION_SENSOR_PIN, INPUT);

  digitalWrite(TEST_PIN, LOW);

  mp3Plyer = new MP3Player(MP3_RX, MP3_TX);
  mp3Plyer->setVolume(5);


  for(int  i = 0; i < 15; i++) {
    //Serial.print(".");
    delay(950);
    digitalWrite(TEST_PIN, HIGH);
    delay(50);
    digitalWrite(TEST_PIN, LOW);
  }
  
  //Serial.println("Ready");
  move_start_ms = millis();
}

void loop() 
{
  unsigned long timeout = millis() - move_start_ms;

  if (state == 0) 
  {
    if (wakeUp())
    {
      state = 1;
      digitalWrite(TEST_PIN, HIGH);
      
      //mp3Plyer->reset();
      delay(100);
      mp3Plyer->play();
      delay(100);
      move_start_ms = millis();
      return;
    }
    testBlink->update();

    delay(100);
    return;
  }

  updateFades();
  updateServo();

  delay(1);
  if (timeout <= MOVING_TIME * 1000) {
    return; 
  }
  move_start_ms = millis();

  if (state == 1)
  {
    state = 2;
    return;
  }

  digitalWrite(TEST_PIN, LOW);
  state = 0;
  delay(10);
  blink_start_ms = millis() + 3000;
  testBlink->reset();
}

void updateFades() 
{
  eyesFade->update(state);
  lanternFade->update(state);
}

void updateServo()
{
  servo->update(state);
}

bool wakeUp() 
{
  return digitalRead(MOTION_SENSOR_PIN) == HIGH || digitalRead(BTN_PLAY) == HIGH;
  //int pinState = digitalRead(BTN_PLAY) == HIGH;
  //int pinState = digitalRead(MOTION_SENSOR_PIN);
  //Serial.print("Sensor: ");
  //Serial.println(pinState);
  //return pinState == HIGH;
}


