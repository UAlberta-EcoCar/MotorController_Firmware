#include "esc_can.h"
#include "esc.h"
#include "sensors.h"
#include "hardware.h"

//Create objects for classes
Servo motor;
Can myCan;
Sensors mySensors;
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

///////Testing variables///////
// uint16_t test_speed = 20; // To be used with testing motor without throttle values
bool test_flag = false;
/////End Testing Variables/////

void setup() {
  Serial.begin(BR); // Initialize Serial Output
  // while (!Serial); //holds program up until Serial is working
  myEsc.begin(); // ONE

  // myCan.begin(); // Initialize CAN - Currently does nothing, talk to Reagan

  pinMode(CAN_INT, INPUT); // Initialize CAN pin
  pinMode(enc1, INPUT); // Initialize Encoder pins
  // pinMode(enc2, INPUT);
  // pinMode(enc3, INPUT); // End Encoder pin initialization
  pinMode(curr_sens_pin, INPUT); // Initialize Current Pin
  pinMode(pinger_pin, INPUT); // Initialize Pinger Pin
}

void loop() {
/////////////////Check CAN////////////////////////
      myCan.read(); //TWO

   if(digitalRead(CAN_INT) == 0) //If there was a "message received interrupt"
   {
     digitalWrite(led1, !digitalRead(led1));
     Serial.print("Read something");
     Serial.println(millis());
   }
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
  //  curr_val = mySensors.mcurrent(curr_sens_pin); // Attain current value
  //  Serial.print("Current Value: ");
  //  Serial.println(curr_val);
  //
  //  myCan.send_mcurrent(curr_val); // Send current value via CAN

/////////////////Speed////////////////////////
  //  velocity = mySensors.mspeed(enc1_count, enc_inc, enc1_avg, enc1_val, enc1_hold, count, wheel_diam, gear_rat, runtime, speedtimer);
   //
  //  myCan.send_mspeed(velocity);

/////////////////Pinger////////////////////////
  //  distance = mySensors.pinger(XX);

  //  myCan.send_pinger(distance);
}
