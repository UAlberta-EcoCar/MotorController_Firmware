#include <Servo.h> 

// setup servo library
Servo esc;  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 

//setup position and speed variables
unsigned int speed_max; // variable to set servo speed
int button = 0; // emergency on or off variable
int baud = 2400; // set data transmission frequency

//setup analog and digital pins
int pot = 9; // analog input from potentiometer
int servo = 10; // pin to connect to ESC orange line
int onoff = 8; // digital input for button 

void setup() 
{ 
  pinMode(pot,INPUT);
  pinMode(onoff, INPUT);
  esc.attach(servo);  // attaches the servo on pin 10 to the servo object
  //ESC needs a low high low signal to turn on
  esc.write(0);
  delay(1000);
  esc.write(180); //gives about 1/2 duty cycle. So I guess assume 360 means full duty cycle
  delay(500);
  esc.write(0);
  delay(500);
  Serial.begin(baud); //control speed through serial
  Serial.setTimeout(500); //Maximum time to wait for serial data set to 500ms (default is 1000ms)
  speed_max=1;
} 

unsigned int num;
void loop() 
{
//  speed_max = analogRead(pot);
//  button = digitalRead(onoff);
//  int speed_act = (speed_max)/6; // adjusting the potentiometer input to servo input range (0 - 180 I believe) with a safety floor (the -100)s
//  Serial.print("Actual Input: ");
//  Serial.println(speed_act);
//  Serial.print("Pot Input: ");
//  Serial.println(speed_max);
//  Serial.print("button: ");
//  Serial.println(button);
//    if (button = 0)
//    {
//      if (speed_act > 90) // set a safety cap so the motor doesn't spin at more than 50%
//      {
//         esc.write(1);
//      }
//      else (speed_act >= 2)
//      {
//           esc.write(speed_act);
//      }
//    else
//    {
//      esc.write(1);
//    }
// Useful operation -- Debounce --
  num = Serial.parseInt();
  Serial.println(speed_max);
  esc.write(speed_max);
  if (num != 0)
  {
    speed_max = num;
  }
  delay(1500);
}
