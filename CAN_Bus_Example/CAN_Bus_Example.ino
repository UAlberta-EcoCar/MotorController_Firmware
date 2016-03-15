//this sketch simulates a node in the CAN network

#include <mcp2515_lib.h>
#include <can_message.h>
#include <can_message_def.h>

/*
 * - This node will be sending:
 *      - MESSAGE_FC_LOGGING_FCTEMP_ID twice a second
 *      - MESSAGE_FC_LOGGING_FCVOLT_FCCURR_ID four times a second
 *      - MESSAGE_FC_LOGGING_CAPVOLT_CAPCURR_ID 5 times a second
 */

can_msg::MsgEncode throttle_msg( can_msg::UINT16, can_msg::MOTOR, can_msg::THROTTLE, can_msg::CRITICAL, 1 );

unsigned char var = 0;
#define NL Serial.println();
#define SPACE Serial.print(" ");

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial);

 // Initialize CAN
 Serial.print("Initializing mcp2515 CAN controller... ");
  if (can_init(0,0,0,0,0,0,0,0)){ //filter set to accept all messages
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
    if(message.id == throttle_msg.id()) //if message is a throttle message
    {
      int throttle = message.data[0] | (message.data[1] << 8); //read data
      Serial.println(throttle); //display data
    }
    
  }
  else
  {
    
  }
}






