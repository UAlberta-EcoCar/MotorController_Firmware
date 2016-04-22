#ifndef ESC_CAN_H
#define ESC_CAN_H

#include <Arduino.h>
// #include <mcp2515_lib.h> //For use with ATMEGA chip
// #include <mcp2515_filters.h>
#include <FlexCAN.h>
#include <can_message.h>
#include <can_message_def.h>
#include "hardware.h"

class Can {
public:
  Can() {};
  void begin();

  void send_mcurrent(uint16_t mcurrent);
  void send_mspeed(uint16_t mspeed);
  void send_pinger(uint16_t pinger);

  void read();
  bool throttle_available() {return _throttle_flag;}
  uint16_t throttle() {_throttle_flag = false; return _throttle;}
  bool brake_available() {return _brake_flag;}
  uint16_t brake() {_throttle_flag = false; return _brake;}

private:
  bool _throttle_flag; uint16_t _throttle;
  bool _brake_flag; uint16_t _brake;
};

#endif
