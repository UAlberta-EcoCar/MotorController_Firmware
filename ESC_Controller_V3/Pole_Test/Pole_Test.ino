#include <Arduino.h>
#include <Servo.h>

int enc1_count = 0;
int enc1_val = 0;
int enc1_hold = 0;
int pin = 2;
int test;

const int servo_pin = 5; //Set servo pin
Servo esc;

void setup() {
    esc.attach(servo_pin); //ESC needs a low high low signal to turn on
    esc.write(0);
    delay(1000);
    esc.write(180);
    delay(500);
    esc.write(0);
    delay(500);
  
  pinMode(pin, INPUT);
  Serial.begin(9600);
}

void loop() {
  enc1_val = digitalRead(pin);

  if (enc1_val) { /*Setup counter to measure velocity - Note that we need a sampling rate of minimum 15000 rpm = 250 rps = 1750 poles per second */
      if (enc1_hold == 0) {
        enc1_hold = 1;
        enc1_count++;
        
        if (enc1_count >= 100) {
          test = millis();
          Serial.println(test);
//          velocity = (enc1_count*60000/(7*(millis())));
          enc1_count = 0;
        }
      }
    }
    else {
      enc1_hold = 0;
    }

  if (millis() < 20000) {
    esc.write(20);
  }
  else {
    esc.write(0);
  }
}
