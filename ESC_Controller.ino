#include <Servo.h> 

Servo esc;  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 

int pos = 0;    // variable to store the servo position 
unsigned int speed_max;
void setup() 
{ 
  esc.attach(10);  // attaches the servo on pin 10 to the servo object
  //ESC needs a low high low signal to turn on
  esc.write(0);
  delay(1000);
  esc.write(180); //gives about 1/2 duty cycle. So I guess assume 360 means full duty cycle
  delay(500);
  esc.write(0);
  delay(500);
  Serial.begin(9600); //control speed through serial
  Serial.setTimeout(500); //Maximum time to wait for serial data set to 500ms (default is 1000ms)
  speed_max=10;
} 

unsigned int num;
void loop() 
{
  num = Serial.parseInt();
  Serial.println(speed_max);
  esc.write(speed_max);
  if (num != 0)
  {
    speed_max = num;
  }
  delay(15);
}
