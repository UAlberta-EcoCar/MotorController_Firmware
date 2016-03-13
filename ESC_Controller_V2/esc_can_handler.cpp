#include <stdio.h>
#include "can_message.h"
#include "can_message_def.h"
#include "esc_can_handler.h"
#include <mcp2515_lib.h>
#include <Arduino.h>
#include <mcp2515_filters.h>

can_msg::MsgEncode throttle_msg( can_msg::UINT8, can_msg::MOTOR, can_msg::THROTTLE, can_msg::CRITICAL, 1 );
can_msg::MsgEncode brake_msg( can_msg::UINT8, can_msg::MOTOR, can_msg::BRAKE, can_msg::CRITICAL, 1 );
can_msg::MsgEncode merror_msg( can_msg::BOOL, can_msg::MOTOR, can_msg::MERROR, can_msg::IMPORTANT, 2 );
can_msg::MsgEncode mspeed_msg( can_msg::INT16, can_msg::MOTOR, can_msg::MSPEED, can_msg::INFORMATION, 1 );
can_msg::MsgEncode mcurrent_msg( can_msg::INT16, can_msg::MOTOR, can_msg::MCURRENT, can_msg::INFORMATION, 1 );


void esc_can_init(void)
{
    while(can_init((DEVICE_MASK|MESSAGE_MASK),brake_msg.id(),throttle_msg.id(),(DEVICE_MASK|MESSAGE_MASK),0,0,0,0))
    {
        delay(100);
    }
}

void send_speed(uint16_t val) {
  // send brake
  CanMessage msg;
  msg.id = mspeed_msg.id();
  msg.length = mspeed_msg.len();
  mspeed_msg.buf(msg.data, val);
  can_send_message(&msg);
}

void send_current(uint16_t val) {
  // send brake
  CanMessage msg;
  msg.id = mcurrent_msg.id();
  msg.length = mcurrent_msg.len();
  mcurrent_msg.buf(msg.data, val);
  can_send_message(&msg);
}

void set_error(bool stall, bool overcurrent) {
  // send error
  msg.id = merror_msg.id();
  msg.len = merror_msg.len();
  merror_msg.buf(msg.buf, stall << can_msg::MERROR_STALL | overcurrent << can_msg::MERROR_OVERCURRENT);
  CANTx.write(msg);
}

uint16_t throttle_val;
uint16_t get_throttle_val(void)
{
  return(throttle_val);
}
uint8_t brake_val;
uint8_t get_brake_val(void)
{
  return(brake_val);
}
void read_can_bus(void)
{
  CanMessage message;
  message = can_get_message();
  //filter through message ID's
  if(message.id == throttle_msg.id())
  {
    throttle_val = message.data[0] | (message.data[1] << 8);
  }
  else if (message.id == brake_msg.id())
  {
    brake_val = message.data[0];
  }
}

