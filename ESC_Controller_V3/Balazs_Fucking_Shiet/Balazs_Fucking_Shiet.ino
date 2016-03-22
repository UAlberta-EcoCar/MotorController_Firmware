#include <Arduino.h>
#include <Servo.h>
const int servo_pin = 5; //Set servo pin
Servo esc;

int p = 100;
int i = 0;
int vals = 0;
int readCounter = 0;
int encpin = 2;
int hold = 0;
int count = 0;
uint16_t velocity = 0;
uint32_t RunTime = 0;

void setup() {
  pinMode(encpin, INPUT);
  Serial.begin(9600);
  
  esc.attach(servo_pin); //ESC needs a low high low signal to turn on
  esc.write(0);
  delay(1000);
  esc.write(180);
  delay(500);
  esc.write(0);
  delay(500);
}

void loop() {
  vals = 0;
  readCounter++;
  
  for (i = 0; i<p; i++) {
    vals = vals + digitalRead(encpin);
  }
  if (vals > p/2) {
    if (hold == 0) {
      count++;
      hold = 1;
    }
  }
  else {
    hold = 0;
  }

  if (count > 63 || readCounter > 4000) {
    velocity = (count*60000)/(7*(millis()-RunTime));
    Serial.print("count: ");
    Serial.println(count);
    Serial.print("readCounter: ");
    Serial.println(readCounter);
    count = 0;
    readCounter = 0;
    Serial.print("Velocity: ");
    Serial.println(velocity);
    RunTime = millis();
  }
  
  if (millis() < 20000) {
    esc.write(6);
  }
  else {
    esc.write(5);
  }
}
