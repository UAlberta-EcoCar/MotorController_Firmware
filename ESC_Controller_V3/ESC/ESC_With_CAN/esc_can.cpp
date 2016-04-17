#include "esc_can.h"

// CAN message definitions
// Throttle
can_msg::MsgEncode throttle_msg( can_msg::UINT16, can_msg::MOTOR, can_msg::THROTTLE, can_msg::CRITICAL, 1);
// Brake
can_msg::MsgEncode brake_msg( can_msg::BOOL, can_msg::MOTOR, can_msg::BRAKE, can_msg::IMPORTANT, 1);
// Motor Current -- What does the number at the end of the current_msg mean? Ans - # of information points sent (ie wiper with signalleft and signalright would have the number 2)
can_msg::MsgEncode mcurrent_msg( can_msg::UINT16, can_msg::MOTOR, can_msg::MCURRENT, can_msg::INFORMATION, 1);
// Speed
can_msg::MsgEncode mspeed_msg( can_msg::UINT16, can_msg::MOTOR, can_msg::MSPEED, can_msg::INFORMATION, 1);
// Pinger
//can_msg::MsgEncode pinger_msg( can_msg::UINT16, can_msg::MOTOR, can_msg::PINGER, can_msg::INFORMATION, 1);

void Can::begin() {
  //Initialize CAN
  Serial.println("Initializing CAN Controller");
  if (can_init(0,0,0,0,0,0,0,0)){  //Used to filter messages coming in, Reegan is working on this
    Serial.println("Error: CAN Initialization :(");
    while(1); //hold up program while
  }
  Serial.println("CAN Controller Initialized :)");
}

/*
 *Write to CAN Bus
 */

//Send CAN motor current message
void Can::send_mcurrent(uint16_t mcurrent) {
  CanMessage message;
  message.id = mcurrent_msg.id();
  message.length = mcurrent_msg.len();
  mcurrent_msg.buf(message.data, mcurrent);
  can_send_message(&message);
}

//Send CAN motor speed message
void Can::send_mspeed(uint16_t mspeed) {
  CanMessage message;
  message.id = mspeed_msg.id();
  message.length = mspeed_msg.len();
  mspeed_msg.buf(message.data, mspeed);
  can_send_message(&message);
}

//Send CAN pinger message - warns driver of objects behind them
// void Can::send_pinger(uint16_t pinger) {
//   CanMessage message;
//   message.id = pinger_msg.id();
//   message.length = pinger_msg.len();
//   pinger_msg.buf(message.data, pinger);
//   can_send_message(&message);
// }

/*
 *Read CAN Bus
 */

void Can::read() {
  if(!digitalRead(CAN_INT)) {
    CanMessage message = can_get_message();
    //read throttle input
    if (message.id == throttle_msg.id()) {
      _throttle = message.data[0];
      _throttle_flag = true;
    }
    //read brakes input
    if (message.id == brake_msg.id()) {
      _brake = message.data[0];
      _brake_flag = true;
    }
    }
  }
