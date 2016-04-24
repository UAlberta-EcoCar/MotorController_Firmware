#include "esc.h"

uint16_t publicThrottle;

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
	delay(5000);
	Serial.println("Electronic Speed Controller Initialization Successful");
}
void Esc::beginTest() {
  _esc = new Servo();

  //Initilize ESC
	Serial.println("Initializing ESC");
	_esc->attach(servo_pin); //ESC needs a low high low signal to turn on
	_esc->write(0);
	delay(1000);
	_esc->write(180);
	delay(500);
	_esc->write(0);
	delay(5000);
	Serial.println("Electronic Speed Controller Initialization Successful");
}

void Esc::write(uint16_t throttle) {
  /*
   *Alice code: for use when the throttle or a potentiometer is attached
   *
   *throttle_val = ((throttle - zero'ing_factor)*motor_maxvalue/calculated_value_to_set_max_desired_servo_value)
   */

  //int throttle_val = ((940 - throttle)/(940-263))*180; //
	publicThrottle = throttle;
  _esc->write(throttle);

}

void Esc::reattach(uint16_t newMax){
	_esc->detatch();
	_esc->attach(servo_pin,544,newMax);
	_esc->write(publicThrottle);
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
