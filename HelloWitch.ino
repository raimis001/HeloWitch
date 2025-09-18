
#include <Servo.h>

#define SERVO_PIN 9 // Arduino Nano pin D9 connected to the signal pin of servo motor
#define MOTION_SENSOR_PIN  2
#define EYES_PIN 6

Servo servo;  // create servo object to control a servo

int MOVING_TIME = 10;
int DELAY_1 = 15; //Delay between revesre state
int DELAY_2 = 20; //Delay between start cycle

unsigned long move_start_ms;

int motion_state   = LOW; // current state of pin


double servoConst = 65.00 / 90.00;

int angle_start = 0; // 30°
int angle_stop = 90 * servoConst; // 90°

int state = -1; //-1 setup 0 waiting 1 opening 2 closing

float eyesProgress = 0.0;   
int eyesBrightness = 1;  // how bright the LED is
float eyesFadeAmount = 0.0001;  // how many points to fade the LED by


void Start_Servo() 
  {
    servo.write(angle_stop);

    delay(3000);
    Serial.print("Servo setup: ");
    Serial.println(servo.read());
    state = 0;
  }

  void Eyes_Lights()
  {
    if (state < 1)
      return;

    if (state == 1)
    {
      if (eyesProgress >= 1.0) return;
      if (eyesProgress < 1.0) eyesProgress += eyesFadeAmount;
      if (eyesProgress > 1.0) eyesProgress = 1.0;
    }

    if (state == 2) 
    {
      if (eyesProgress <= 0.0) return;
      if (eyesProgress > 0.0) eyesProgress -= eyesFadeAmount;
      if (eyesProgress < 0.0) eyesProgress = 0.0;
    }

    eyesBrightness = (int)(255.0 * (eyesProgress * eyesProgress));
    if (eyesBrightness <= 0) eyesBrightness = 1;
    
    
    //Serial.println(eyesBrightness);
    analogWrite(EYES_PIN, eyesBrightness);
    delay(1);
  }

void Check_Motion()
{
  if (state != 0)
    return;

  int prev_motion_state = motion_state; // store old state
  motion_state = digitalRead(MOTION_SENSOR_PIN);   // read new state

  if (prev_motion_state == LOW && motion_state == HIGH) {   // pin state change: LOW -> HIGH
    Serial.println("Motion detected!");
    digitalWrite(LED_BUILTIN, HIGH);
    state = 1;
    move_start_ms = millis();
  }

  /*
  else if (prev_motion_state == HIGH && motion_state == LOW) {  // pin state change: HIGH -> LOW
    Serial.println("Motion stopped!");
    digitalWrite(LED_BUILTIN, LOW);
    state = 0;
  }
  */
}

void Operate_Servo(unsigned long progress) {
  long angle = 0; 
  if (state == 1) {
    angle = map(progress, 0, MOVING_TIME * 1000, angle_stop, angle_start);
  } else {
    angle = map(progress, 0, MOVING_TIME * 1000, angle_start, angle_stop);
  }
  servo.write(angle);
  delay(1);
}

void setup() {
  Serial.begin(9600);  
  

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(MOTION_SENSOR_PIN, INPUT);
  pinMode(EYES_PIN, OUTPUT);

  servo.attach(SERVO_PIN);  // attaches the servo on pin 9 to the servo object

  analogWrite(EYES_PIN, eyesBrightness);

  move_start_ms = millis();  
}


void loop() {

  if (state == -1) {
    Start_Servo();
    return;
  }

  if (state == 0)
  {
    Check_Motion();
    return;
  }

  Eyes_Lights();

  unsigned long progress = millis() - move_start_ms;
  if (progress <= MOVING_TIME * 1000) {
    Operate_Servo(progress);
    return; //TODO servo moving
  }

  if (state == 1) //Reverse state
  {
    delay(DELAY_1 * 1000);
    state = 2;
    move_start_ms = millis();
    return;
  }

  //Return to waiting state
  delay(DELAY_2 * 1000);
  state = 0;
  motion_state = LOW;
  digitalWrite(LED_BUILTIN, LOW);

}
