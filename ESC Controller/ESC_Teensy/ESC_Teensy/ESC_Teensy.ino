// #include "esc_can.h"
#include "esc.h"
#include "sensors.h"
#include "hardware.h"
#include <FlexCAN.h>


//Create objects for classes
Esc myEsc;


uint16_t curr_val = 0; // Translates voltage to current
uint16_t velocity; // Translates encoder values to speed
int enc1_val = 0; // Initiate variables to calculate speed
int enc_inc = 0;
int enc1_count = 0;
int enc1_avg = 0;
int count = 0;
int enc1_hold = 0;
uint16_t runtime = 0; // End speed calculation variables
uint16_t throttle = 0; // Analog value of gas pedal
uint16_t brake = 0; // Analog value of brake pedal

int throttle_val = 10;
int rawIn = 10;
int rollingValues[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


///////Testing variables///////
// uint16_t test_speed = 20; // To be used with testing motor without throttle values
bool test_flag = false;
/////End Testing Variables/////


/*int led = 13;
FlexCAN CANbus(500000);
static CAN_message_t msg,rxmsg;
static uint8_t hex[17] = "0123456789abcdef";

int txCount,rxCount;
unsigned int txTimer,rxTimer;*/

static CAN_message_t msg;
FlexCAN CANTransmitter(500000);


int averageValue(){
	int v = 0;
	for(int i = 0; i < 20; i++){
		v = v+rollingValues[i];
	}
	return (v/20);
}

void addValue(int value){
	for(int i = 1; i < 20; i++){
		rollingValues[i] = rollingValues[i-1];
	}
	rollingValues[0] = value;
}

void averageValueTest(){
	Serial.println("Start Test");
	addValue(2);
	addValue(4);
	addValue(8);
	int e = averageValue();
	if (e != 1){
		Serial.println("Error");
		Serial.print("Result: ");
		Serial.println(averageValue());
	}
}

void setup() {
	/*
	CANbus.begin();
	  pinMode(led, OUTPUT);
	  digitalWrite(led, 1);

	  delay(1000);
	  Serial.println(F("Hello Teensy 3.1 CAN Test."));*/
	Serial.begin(BR); // Initialize Serial Output
	Serial.println("Starting delay");
	delay(20000);
	//analogReadRes(1);
	//pinMode(A11,INPUT);

	//CANTransmitter.begin();
  //pinMode(led, OUTPUT);
  delay(1000);
  Serial.println("CAN Transmitter Initialized");

  Serial.begin(BR); // Initialize Serial Output
  // while (!Serial); //holds program up until Serial is working
  myEsc.begin(); // ONE

  //CANbus.begin();

  // myCan.begin(); // Initialize CAN - Currently does nothing, talk to Reagan

  // pinMode(CAN_INT, INPUT); // Initialize CAN pin
  pinMode(enc1, INPUT); // Initialize Encoder pins
  // pinMode(enc2, INPUT);
  // pinMode(enc3, INPUT); // End Encoder pin initialization
  pinMode(curr_sens_pin, INPUT); // Initialize Current Pin
  pinMode(pinger_pin, INPUT); // Initialize Pinger Pin
}

void loop() {
	rawIn = analogRead(A11);
	throttle_val = (rawIn * 180) /1023;
	//Deploy
	Serial.println(throttle_val);
	myEsc.write(throttle_val);
	delay(50);

/* Uncomment me to send throttle values
	Serial.print("Sending: ");
  msg.id = 0x222;
  msg.len = 2;
  for(int i=0; i<msg.len; i++) {
    msg.buf[i] = '0' + i;
    Serial.println(msg.buf[i]);
  }
  CANTransmitter.write(msg);


*/





	/*
  // if not time-delayed, read CAN messages and print 1st byte
  while ( CANbus.read(rxmsg) ) {
    //hexDump( sizeof(rxmsg), (uint8_t *)&rxmsg );
    Serial.write(rxmsg.buf[0]);

  }
*/






/////////////////Check CAN////////////////////////
  // myCan.read(); //TWO

  //  if(digitalRead(CAN_INT) == 0) //If there was a "message received interrupt"
  //  {
  //    digitalWrite(led1, !digitalRead(led1));
  //    Serial.print("Read something");
  //    Serial.println(millis());
  //  }
//////////////Throttle and Brake//////////////
  // if (myCan.throttle_available()) {
  //   throttle = myCan.throttle();
  //   Serial.print("Throttle: ");
  //   Serial.println(throttle);
  // }
  //
  // if (myCan.brake_available()) {
  //   brake = myCan.brake();
  //   Serial.print("Brake: ");
  //   Serial.println(brake);
  // }
  //
  // if (brake < 50) {
  //   myEsc.write(throttle);
  //   Serial.println("Throttle Value Deployed");
  // }
  // else {
  //   myEsc.write(0);
  //   Serial.println("Brake Deployed");
  // }

  // myEsc.test(test_speed); //Function used when testing motor without throttle values

/////////////////Current//////////////////////


/*
	if ( !rxTimer ) {
	    while ( CANbus.read(rxmsg) ) {
	      //hexDump( sizeof(rxmsg), (uint8_t *)&rxmsg );
	      Serial.write(rxmsg.buf[0]);
	      //rxCount++;
	    }
	  }
		*/

   /*curr_val = mySensors.mcurrent(curr_sens_pin); // Attain current value
   msg.message = curr_val;
   msg.len = 8;
   msg.id = 0x222;
   for( int idx=0; idx<8; ++idx ) {
    msg.buf[idx] = '0'+idx;
  }
   Serial.print("Current Value: ");
   Serial.println(curr_val);

   int txCount = 6;
   Serial.println(".");
   while ( txCount-- ) {
     CANbus.write(msg);
     msg.buf[0]++;
   }*/
  //
  //  myCan.send_mcurrent(curr_val); // Send current value via CAN
   //CANbus.write(curr_val); // Teensy CAN code

/////////////////Speed////////////////////////
  //  velocity = mySensors.mspeed(enc1_count, enc_inc, enc1_avg, enc1_val, enc1_hold, count, wheel_diam, gear_rat, runtime, speedtimer);
   //
  //  myCan.send_mspeed(velocity);

/////////////////Pinger////////////////////////
  //  distance = mySensors.pinger(XX);

  //  myCan.send_pinger(distance);
}
