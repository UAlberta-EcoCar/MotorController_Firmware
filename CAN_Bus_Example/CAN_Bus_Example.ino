//this sketch simulates a node in the CAN network

#include <mcp2515_lib.h>
#include <mcp2515_filters.h>
#include <can_message.h>
#include <can_message_def.h>

//Create throttle_msg
can_msg::MsgEncode throttle_msg( can_msg::UINT16, can_msg::MOTOR, can_msg::THROTTLE, can_msg::CRITICAL, 1 );
//Create brake_msg
can_msg::MsgEncode brake_msg( can_msg::UINT16, can_msg::MOTOR, can_msg::BRAKE, can_msg::CRITICAL, 1 );

unsigned char var = 0;
#define NL Serial.println();
#define SPACE Serial.print(" ");

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial);

 // Initialize CAN
 Serial.print("Initializing mcp2515 CAN controller... ");
  if (can_init(0,0,0,0,0,0,0,0)){ //filter set to accept throttle and brake msg's
    Serial.println("mcp2515 config error");
    while(1); //hang up program
  }
  Serial.println("mcp2515 initialization successful");

  pinMode(2,OUTPUT);
  
  // Short delay and then begin communication
  delay(2000);
}


void loop() 
{ 
  CanMessage message; //make a new message object
  if(digitalRead(9) == 0) //If there was a "message received interrupt" (happy mike?)
  {
    message = can_get_message(); //retrieve message
    if(message.id == throttle_msg.id()) //if message is a throttle message (the "&(DEVICE_MASK|MESSAGE..)" makes code only check message and device parts of the ID
    {
      int throttle = message.data[0] | (message.data[1] << 8); //read data
      Serial.println(throttle); //display data
    }
    else if((message.id&(DEVICE_MASK|MESSAGE_MASK)) == (brake_msg.id()&(DEVICE_MASK|MESSAGE_MASK))) //if message is a throttle message (the "&(DEVICE_MASK|MESSAGE..)" makes code only check message and device parts of the ID
    {
      int brake = message.data[0] | (message.data[1] << 8); //read data
      Serial.print("     ");
      Serial.println(brake); //display data
    }
    
  }
  else
  {
    
  }
}






