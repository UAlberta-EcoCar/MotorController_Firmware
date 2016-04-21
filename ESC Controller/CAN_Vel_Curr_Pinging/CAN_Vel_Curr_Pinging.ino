#include "esc_can.h"
#include "esc.h"
#include "hardware.h"

//Create servo object as declared in servo.h library
Servo motor;
Esc myEsc;
Can myCan;

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

uint16_t can_time_timer = 0; //CAN time holder
uint16_t time_write_interval = 200; //How often do you want to send values over can?
uint32_t runtime = 0; //To be attached to millis()
uint32_t disp_speed = 0; //Variable used to hold millis value to control serial display speed

uint16_t velocity = 0; // Set storage variable for total speed in rpm after calculations
uint16_t throttle; // Raw pedal (potentiometer) data
uint16_t brake; //Raw brake (potentiometer) data
int throttle_val = 0; // Value between 1-180 that goes into esc.write
float curr_sens_voltage = 0; //Set storage variable for mV output of current sensor
float curr_val; //Translates voltage to current

void setup() {
  Serial.begin(BR); // Initialize Serial Output
  // while (!Serial); //holds program up until Serial is working

  myEsc.begin(); // Initialize Electronic Speed Controller

  myCan.begin(); // Initialize CAN

//  myCan.send_mcurrent(false);
//  myCan.send_mspeed(false);
//  myCan.send_pinger(false);

  pinMode(enc1, INPUT); //initialise encoder pins
  pinMode(enc2, INPUT);
  pinMode(enc3, INPUT);
  pinMode(curr_sens_pin, INPUT); //initialise current pins
  pinMode(led1,OUTPUT); //initialise LED pins
  pinMode(led2,OUTPUT);
  pinMode(led3,OUTPUT);
  pinMode(CAN_INT, INPUT);

}

void loop() {
  myCan.read(); // Setup to read the CAN Bus every loop

///////////////////////////////////////////////////////////////////////////////////

   curr_sens_voltage = analogRead(curr_sens_pin)-303; //attain current sensor value
   curr_val = curr_sens_voltage/2.71;

 //   if(millis() - can_time_timer > time_write_interval) {
 //   if (curr_val < 1) { //crudely set the current to read zero if nothing is attached
 //    curr_val = 0;
 //   }
 //   myCan.send_mcurrent(curr_val);
 // }

////////////////////////////////////////////////////////////////////////////////////

      // CanMessage message; //make a new message object
  if(digitalRead(CAN_INT) == 0) //If there was a "message received interrupt" (happy mike?)
  {
    digitalWrite(led1, !digitalRead(led1));
    Serial.print("Read something");
    Serial.println(millis());
  }
    // if (digitalRead(led1) == 0  &&  (millis() - led1_hold > 1000)) {
    //   digitalWrite(led1, HIGH);
    //   led1_hold = millis();
    //   Serial.print("LEDH");
    // }
    // if (digitalRead(led1) == 1 && (millis() - led1_hold > 1000)) {
    //   digitalWrite(led1, LOW);
    //   led1_hold = millis();
    //   Serial.print("LEDL");
    // }
    // message = can_get_message(); //retrieve message
    // if(message.id == throttle_msg.id()) { //if message is a throttle message (the "&(DEVICE_MASK|MESSAGE..)" makes code only check message and device parts of the ID
    //   throttle = message.data[0] | (message.data[1] << 8); //read data
    // }
    // else if((message.id&(DEVICE_MASK|MESSAGE_MASK)) == (brake_msg.id()&(DEVICE_MASK|MESSAGE_MASK))) { //if message is a throttle message (the "&(DEVICE_MASK|MESSAGE..)" makes code only check message and device parts of the ID
    //   brake = message.data[0] | (message.data[1] << 8); //read data
    //   if (brake > 50) {
    //     esc.write(0);
    // }
    // }

  // throttle_val = ((throttle - 80)*180)/(744); //denom calc'd by (700-80)*180/highestspeed(ie10)
  // motor.write(throttle_val);
//  Serial.print("throttle: "); //Use to debug speed calculations
//  Serial.print(throttle);
//  Serial.print(" throttle val: ");
//  Serial.println(throttle_val);


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
  ///////////////////////////////////////////////////////////////////////////////////
  // Velocity input code

    if(myCan.throttle_available()) {
    throttle_val = myCan.throttle();
  }
    if(myCan.brake_available()) {
    brake = myCan.brake();
  }
    if(!brake) {
    myEsc.write(throttle_val);
    Serial.print("Throttle: ");
    Serial.println(throttle_val);
    }
    else {
    myEsc.write(0);
    }
  ///////////////////////////////////////////////////////////////////////////////////
  /* Velocity Measuring Code - Hall Effect Sensors
   * current issue - velocity isn't accurate but it appears to be linear so we will try to scale the overall velocity
   * to fit the right km/h
   */
      // enc1_val = 0;
      // enc1_count++;
      //
      // for (enc_inc = 0; enc_inc<enc1_avg; enc_inc++) {
      //   enc1_val = enc1_val + digitalRead(enc1);
      // }
      // if (enc1_val > enc1_avg/2) {
      //   if (enc1_hold == 0) {
      //     count++;
      //     enc1_hold = 1;
      //   }
      // }
      // else {
      //   enc1_hold = 0;
      // }
      //
      // if (count > 63 || enc1_count > 4000) {
      //   velocity = (count*3600)*(3.14*wheel_diam)/(gear_rat*7*(millis()-runtime));
      //   count = 0;
      //   enc1_count = 0;
      //   runtime = millis();
      //   if (millis() - speedtimer > 500) {
      //     Serial.print("Velocity: ");
      //     Serial.println(velocity);
      // }
      // }
      if(millis() - can_time_timer > time_write_interval) {
      velocity = 50;
      myCan.send_mspeed(velocity);
      can_time_timer = millis();
    }
      // if (millis() - speedtimer > 500) {
      //   myCan.send_mspeed(velocity);
      //   Serial.print("Velocity: ");
      //   Serial.print(velocity);
      //   speedtimer = millis();
      //   velocity++;
      // }
}
