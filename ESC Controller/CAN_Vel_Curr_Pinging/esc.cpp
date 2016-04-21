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
  delay(500);
  Serial.println("Electronic Speed Controller Initialization Successful");
}

void Esc::write(uint16_t throttle) {
  /*
   *Alice code: for use when the throttle or a potentiometer is attached
   *
   *throttle_val = ((throttle - zero'ing_factor)*motor_maxvalue/calculated_value_to_set_max_desired_servo_value)
   */

  throttle_val = ((throttle - 80)*180)/(744); //
  _esc->write(throttle_val);

  /*
   *Testing code which runs the motor at speed (x) for 20 seconds and then turns off
   */
  //write speed to electronic speed Controller
  // if (millis() < 20000) {
  //   _esc->write(30);
  // }
  // else {
  //   _esc->write(0);
  // }

}
