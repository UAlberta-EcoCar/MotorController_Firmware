#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>

class Sensors {
public:

  Sensors() {};
  uint8_t mcurrent(int curr_sens_pin);
  uint8_t mspeed(int enc1_count, int enc_inc, int enc1_avg, int enc1_val, int enc1_hold, int count, int wheel_diam, int gear_rat, uint16_t runtime, uint16_t speedtimer);
  uint8_t pinger();

private:
  int enc1;
  int curr_sens_pin;
  int8_t speedtimer;
  int enc1_val;
  int enc_inc;
  int enc1_count;
  int enc1_avg;
  int count;
  int enc1_hold;
  uint8_t runtime;
  int wheel_diam;
  int gear_rat;
  uint8_t curr_val;
  uint8_t velocity;

};
#endif
