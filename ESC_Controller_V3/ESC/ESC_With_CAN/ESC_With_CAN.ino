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
//Create current_msg
can_msg::MsgEncode current_msg( can_msg::UINT16, can_msg::MOTOR, can_msg::MCURRENT, can_msg::INFORMATION, 1 );

const int enc1 = 2; //set hall effect 1 signal to arduino pin 2
const int enc2 = 3; //set hall effect 2 signal to arduino pin 3
const int enc3 = 4; //set hall effect 3 signal to arduino pin 4
const int curr_sens = A3; //set current sensor signal to arduino pin A3
const int led1 = A2; //set LED pins
const int led2 = A1;
const int led3 = A0;

int enc1_val = 0; //Start set storage variables for encoder values
int enc2_Val = 0;
int enc3_Val = 0;
int enc_inc = 0;
int enc1_count = 0; 
int enc2_count = 0;
int enc3_count = 0; //End set storage variables for encoder values
int enc1_avg = 0;
int count = 0;
int enc1_hold = 0; //Hold encoder values
int enc2_hold = 0; //
int enc3_hold = 0; //
uint32_t led1_hold = 0; //Hold led value
uint32_t led2_hold = 0;
uint32_t led3_hold = 0;

const int servo_pin = 6; //Set servo pin
Servo esc; //Create servo object as declared in servo.h library

uint32_t runtime = 0; //To be attached to millis()
uint32_t runtime_loop = 1000; //Variable used for Millis - runtime(held value)
uint32_t disp_speed = 0; //Variable used to hold millis value to control serial display speed

uint32_t velocity = 0; // Set storage variable for total speed in rpm after calculations
uint16_t throttle; // Raw pedal (potentiometer) data
uint16_t brake; //Raw brake (potentiometer) data
int throttle_val = 0; // Value between 1-180 that goes into esc.write
float curr_sens_voltage = 0; //Set storage variable for mV output of current sensor
float curr_val; //Translates voltage to current

const int BR = 9600; //set Baud Rate - 115200 doesn't work
const int wheel_diam = 444; //Wheel diameter in mm's
const int gear_rat = 28; //Pre set gear ratio

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
  pinMode(curr_sens, INPUT); //initialise current pins
  pinMode(led1,OUTPUT); //initialise LED pins
  pinMode(led2,OUTPUT);
  pinMode(led3,OUTPUT);

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
///////////////////////////////////////////////////////////////////////////////////
/* current issue - velocity isn't accurate but it appears to be linear so we will try to scale the overall velocity
 * to fit the right km/h */
    enc1_val = 0;
    enc1_count++;
  
    for (enc_inc = 0; enc_inc<enc1_avg; enc_inc++) {
      enc1_val = enc1_val + digitalRead(enc1);
    }
    if (enc1_val > enc1_avg/2) {
      if (enc1_hold == 0) {
        count++;
        enc1_hold = 1;
      }
    }
    else {
      enc1_hold = 0;
    }
  
    if (count > 63 || enc1_count > 4000) {
      velocity = (count*3600)*(3.14*wheel_diam)/(gear_rat*7*(millis()-runtime));
      count = 0;
      enc1_count = 0;
      runtime = millis();
    }
    
///////////////////////////////////////////////////////////////////////////////////

   curr_sens_voltage = analogRead(curr_sens)-303; //attain current sensor value
   curr_val = curr_sens_voltage/2.71;

   if (curr_val < 1) { //crudely set the current to read zero if nothing is attached
    curr_val = 0;
   }

////////////////////////////////////////////////////////////////////////////////////
      
      CanMessage message; //make a new message object
  if(digitalRead(9) == 0) //If there was a "message received interrupt" (happy mike?)
  {   
    if (digitalRead(led1) == 0  &&  (millis() - led1_hold > 1000)) {
      digitalWrite(led1, HIGH);
      led1_hold = millis();
      Serial.print("LEDH");
    }
    if (digitalRead(led1) == 1 && (millis() - led1_hold > 1000)) {
      digitalWrite(led1, LOW);
      led1_hold = millis();
      Serial.print("LEDL");
    }
    message = can_get_message(); //retrieve message
    if(message.id == throttle_msg.id()) { //if message is a throttle message (the "&(DEVICE_MASK|MESSAGE..)" makes code only check message and device parts of the ID
      throttle = message.data[0] | (message.data[1] << 8); //read data
    }
    else if((message.id&(DEVICE_MASK|MESSAGE_MASK)) == (brake_msg.id()&(DEVICE_MASK|MESSAGE_MASK))) { //if message is a throttle message (the "&(DEVICE_MASK|MESSAGE..)" makes code only check message and device parts of the ID
      brake = message.data[0] | (message.data[1] << 8); //read data
      if (brake > 50) {
        esc.write(0);
      }
    }
  }

  throttle_val = ((throttle - 80)*180)/(744); //denom calc'd by (700-80)*180/highestspeed(ie10)
  esc.write(throttle_val);
//  Serial.print("throttle: "); //Use to debug speed calculations
//  Serial.print(throttle);
//  Serial.print(" throttle val: ");
//  Serial.println(throttle_val);

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
  speed_msg.buf(message_send.data, velocity); //16bit unsigned int  
  can_send_message(&message_send); // send message
  //set message id
  //for example use id for Fuel cell temperature
  message_send.id = current_msg.id();
  //set message length
  message_send.length = current_msg.len();
  //set whether message is a remote transmit request (it's not)
  message_send.RTransR = 0;
  //set 8 data bytes
  current_msg.buf(message_send.data, curr_val); //16bit unsigned int
  can_send_message(&message_send); // send message
  
//  if (can_send_message(&message_send)) //Prints really fast so I commented it out
//  {
//    Serial.println("Transmition Error: ");
//    //something is wrong and/or all transmition buffers are full.
//  }
//  else
//  {
//    Serial.println("Message sent");
//////////////////////////////////////////////////////////////////////////////////////////
  
  if (millis() - disp_speed > 1000) { //Set MCU to print information every second

//        Serial.println(" ");
//        Serial.print(velocity);
//        Serial.print(" rpm  ");
        Serial.print("  runtime: ");
        Serial.println(runtime);
//        Serial.print("  Encoder 1: ");
//        Serial.print(enc1_val);
        Serial.print("  Brake: ");
        Serial.println(brake);
        Serial.print("  Throttle: ");
        Serial.println(throttle);
        Serial.print("  Current: ");
        Serial.println(curr_val);
        Serial.print("curr_sens_voltage: ");
        Serial.println(curr_sens_voltage);
//        Serial.print("count: ");
//        Serial.print(enc1_count);
//        Serial.print("Encoder 2: ");
//        Serial.println(enc2_Val);
//        Serial.print("Encoder 3: ");
//        Serial.println(enc3_Val);
        disp_speed = millis();
  }
}
///////////////////////////////////////////////////////////////////////////////////////////////

//  if (millis() < 20000) { //used to test motor without CAN
//    esc.write(20);
//  }
//  else {
//    esc.write(0);
//  }
//}


