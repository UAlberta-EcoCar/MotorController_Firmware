#include "esc.h"

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
  delay(2000);
  Serial.println("Electronic Speed Controller Initialization Successful");
}

void Esc::write(uint16_t throttle) {
  /*
   *Alice code: for use when the throttle or a potentiometer is attached
   *
   *throttle_val = ((throttle - zero'ing_factor)*motor_maxvalue/calculated_value_to_set_max_desired_servo_value)
   */

  throttle_val = ((throttle - 80)*150)/(744); //
  _esc->write(throttle_val);

}

void Esc::test(uint16_t test_speed) {
  /*
   *Testing code which runs the motor at speed (x) for 20 seconds and then turns off
   */

  if (Serial.available()){
    test_speed = Serial.parseInt();
  }

  if (millis() < 20000) {
    _esc->write(test_speed);
    Serial.print("Test Speed: ");
    Serial.println(test_speed);
  }
  else if (test_flag == false) {
    _esc->write(0);
    Serial.println("It's Ovaaaa");
    test_flag = true;
  }
}
