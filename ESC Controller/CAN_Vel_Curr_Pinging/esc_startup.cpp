#include "esc_startup.h"

void Esc::begin() {
  _esc = new Servo();

  //Initilize ESC
  Serial.println("Initializing ESC");
  _esc->attach(servo_pin); //ESC needs a low high low signal to turn on
  _esc->write(0);
  delay(1000);
  _esc->write(180);
  delay(500);
  _esc->write(0);
  delay(500);
  Serial.println("Electronic Speed Controller Initialization Successful");
}
