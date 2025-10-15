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
int MOVING_TIME = 20;
int SLEEPING_TIME = 30; 
int SOUND_TIME = 4;

unsigned long move_start_ms;
unsigned long blink_start_ms;

int state = 0; 
int mp3State = 0;

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
  mp3Plyer->setVolume(25);

  //Init sensors
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
    testBlink->update();
    if (timeout < SLEEPING_TIME * 1000) 
    {
      delay(10);
      return; 
    }

    if (wakeUp())
    {
      state = 1;
      mp3State = 0;
      digitalWrite(TEST_PIN, HIGH);
      move_start_ms = millis();
      return;
    }    

    delay(100);
    return;
  }

  updateFades();
  updateServo();
  if (mp3State == 0 && timeout >= SOUND_TIME * 1000) 
  {
    mp3State = 1;
    mp3Plyer->play();
    delay(100);
  }

  delay(1);

  if (timeout <= MOVING_TIME * 1000) {
    return; 
  }
  

  if (state == 1)
  {
    state = 2;
    move_start_ms = millis();
    return;
  }

  digitalWrite(TEST_PIN, LOW);
  state = 0;
  mp3State = 0;
  delay(10);
  blink_start_ms = millis() + 3000;
  testBlink->reset();
  move_start_ms = millis();
    
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


