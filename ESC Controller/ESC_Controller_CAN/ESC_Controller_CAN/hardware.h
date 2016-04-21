#ifndef HARDWARE_H
#define HARDWARE_H

// CAN Bus Connections and Baud Rate
const int CAN_INT = 9;
const int BR = 9600; //set Baud Rate - 115200 doesn't work

// Set Servo Pin
const int servo_pin = 5; //Set servo pin

// Board LED's
const int led1 = A2; //set LED pins
const int led2 = A1;
const int led3 = A0;

// Encoder Control Pins
const int enc1 = 2; //set hall effect 1 signal to arduino pin 2
const int enc2 = 3; //set hall effect 2 signal to arduino pin 3
const int enc3 = 4; //set hall effect 3 signal to arduino pin 4

// Current Control Pin
const int curr_sens_pin = A3; //set current sensor signal to arduino pin A3

// Pinger Control Pin
const int pinger_pin = A5; //set pinger input pin to arduino pin A5

//Motor Spec's
const int wheel_diam = 444; //Wheel diameter in mm's
const int gear_rat = 28; //Pre set gear ratio

#endif
