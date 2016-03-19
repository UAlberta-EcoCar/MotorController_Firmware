#include <Arduino.h>
#include "mcp2515_lib.h"
#include "mcp2515_filters.h"
#include "can_message.h"
#include "can_message_def.h"
#include "spi_lib.h"
#include <Servo.h>

//Create throttle_msg
can_msg::MsgEncode throttle_msg( can_msg::UINT16, can_msg::MOTOR, can_msg::THROTTLE, can_msg::CRITICAL, 1 );
//Create brake_msg
can_msg::MsgEncode brake_msg( can_msg::UINT16, can_msg::MOTOR, can_msg::BRAKE, can_msg::CRITICAL, 1 );
//Create speed_msg
can_msg::MsgEncode speed_msg( can_msg::UINT16, can_msg::MOTOR, can_msg::MSPEED, can_msg::INFORMATION, 1 );

const int enc1 = 2; //set hall effect 1 signal to arduino pin 2
const int enc2 = 3; //set hall effect 2 signal to arduino pin 3
const int enc3 = 4; //set hall effect 3 signal to arduino pin 4

int enc1_val = 0; //Start set storage variables for encoder values
int enc2_Val = 0;
int enc3_Val = 0;

int enc1_count = 0; 
int enc2_count = 0;
int enc3_count = 0; //End set storage variables for encoder values
int enc1_hold = 0; //Hold encoder value

const int servo_pin = 5; //Set servo pin
Servo esc; //Create servo object as declared in servo.h library

uint32_t runtime = 0; //To be attached to millis()
uint32_t runtime_loop = 1000; //Variable used for Millis - runtime(held value)
uint32_t disp_speed = 0; //Variable used to hold millis value to control serial display speed

uint32_t velocity = 0; // Set storage variable for total speed in rpm after calculations
unsigned int throttle; // Raw pedal (potentiometer) data
unsigned int brake; //Raw brake (potentiometer) data
int throttle_val = 0; // Value between 1-180 that goes into esc.write

const int BR = 9600; //set Baud Rate - 115200 doesn't work for some reason

int car_rpm(uint32_t x) { // velocity function
    uint32_t result;
    
    result = (60000/x);
    return result;
}

void setup() {
  esc.attach(servo_pin); //ESC needs a low high low signal to turn on
  esc.write(0);
  delay(1000);
  esc.write(180);
  delay(500);
  esc.write(0);
  delay(500);
  
  Serial.begin(BR);
  while (!Serial); //holds program up until Serial is working
  
  pinMode(enc1, INPUT); //initialise encoder pins
  pinMode(enc2, INPUT);
  pinMode(enc3, INPUT);

// Initialize CAN
 Serial.print("Initializing mcp2515 CAN controller... ");
  if (can_init(0,0,0,0,0,0,0,0)){ //filter set to accept throttle and brake msg's
    Serial.println("mcp2515 config error");
    while(1); //hang up program
  }
  Serial.println("mcp2515 initialization successful");
  
  // Short delay and then begin communication
  delay(2000);
}

void loop() {
////////////////////////////////////////////////////////////////////////////////////////////

    enc1_val = digitalRead(enc1); //Set pins to read value and store in encx_Val variable
    enc2_Val = digitalRead(enc2);
    enc3_Val = digitalRead(enc3);

    if (enc1_val) { //Setup counter to measure velocity
      if (enc1_hold == 0) {
        enc1_hold = 1;
        enc1_count = enc1_count + 1;

        if (millis() - runtime >= runtime_loop) {
          runtime = millis();
          velocity = (enc1_count*60000/(runtime_loop));

          enc1_count = 0;
        }
      }
    }
    else {
      enc1_hold = 0;
    }

//    if (enc1_count >= 7) {
//      runtime_loop = millis() - runtime;
//      runtime = millis();
//      velocity = car_rpm(runtime_loop); //Call velocity function
//
//      enc1_count = 0;
//    }
    
///////////////////////////////////////////////////////////////////////////////////

      CanMessage message; //make a new message object
  if(digitalRead(9) == 0) //If there was a "message received interrupt" (happy mike?)
  {
    message = can_get_message(); //retrieve message
    if(message.id == throttle_msg.id()) //if message is a throttle message (the "&(DEVICE_MASK|MESSAGE..)" makes code only check message and device parts of the ID
    {
      throttle = message.data[0] | (message.data[1] << 8); //read data
    }
    else if((message.id&(DEVICE_MASK|MESSAGE_MASK)) == (brake_msg.id()&(DEVICE_MASK|MESSAGE_MASK))) //if message is a throttle message (the "&(DEVICE_MASK|MESSAGE..)" makes code only check message and device parts of the ID
    {
      brake = message.data[0] | (message.data[1] << 8); //read data
      if (brake > 50) {
        esc.write(0);
      }
    }
  }

  throttle_val = ((throttle - 147)*180)/(1500);
  esc.write(throttle_val);

    //create new message
  CanMessage message_send;
  //set message id
  //for example use id for Fuel cell temperature
  message_send.id = speed_msg.id();
  //set message length
  message_send.length = speed_msg.len();
  //set whether message is a remote transmit request (it's not)
  message_send.RTransR = 0;
  //set 8 data bytes
  //fuel cell controller has two different temperature readings
  speed_msg.buf(message_send.data, velocity); //16bit unsigned int
  
  can_send_message(&message_send); // send message
  
//  if (can_send_message(&message_send)) //Prints really fast so I commented it out
//  {
//    Serial.println("Transmition Error: ");
//    //something is wrong and/or all transmition buffers are full.
//  }
//  else
//  {
//    Serial.println("Message sent");
//  }

//////////////////////////////////////////////////////////////////////////////////////////
  
  if (millis() - disp_speed > 1000) { //Set MCU to print information every second

        Serial.println(" ");
        Serial.print(velocity);
        Serial.print(" rpm  ");
        Serial.print("  runtime: ");
        Serial.print(runtime);
        Serial.print("  Encoder 1: ");
        Serial.print(enc1_val);
        Serial.print("  Brake: ");
        Serial.print(brake);
        Serial.print("  Throttle: ");
        Serial.print(throttle);
//        Serial.print("count: ");
//        Serial.print(enc1_count);
//        Serial.print("Encoder 2: ");
//        Serial.println(enc2_Val);
//        Serial.print("Encoder 3: ");
//        Serial.println(enc3_Val);
        disp_speed = millis();
  }

///////////////////////////////////////////////////////////////////////////////////////////////
}


