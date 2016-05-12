#include <Arduino.h>
#include "hardware.h"

volatile byte revolutions;
float speed;

uint8_t led_toggle = 0;
void rpm_fun()
{
 revolutions++;
 if(led_toggle == 0){
  digitalWrite(led1,HIGH);
  led_toggle = 1;
 }
 else
 {
  digitalWrite(led1,LOW);
  led_toggle = 0;
 }
}


unsigned long timeold;
float WHEELCIRC;

int radius = 0.444;

void setup()
{
 pinMode(led1,OUTPUT);
 pinMode(led2,OUTPUT);
 pinMode(led3,OUTPUT);
  
 Serial.begin(9600);
 delay(1000);

 //pins must be set to input before interrupt is attached
 pinMode(encoder_pin3, INPUT);
 pinMode(encoder_pin2, INPUT);
 pinMode(encoder_pin, INPUT);
 attachInterrupt(encoder_pin2, rpm_fun, RISING);
 revolutions = 0;
 timeold = millis();
 WHEELCIRC = 2 * PI * radius; 
}

void loop()
{
 if (revolutions >= 20) {
   //Update RPM every 20 counts, increase this for better RPM resolution,
   //decrease for faster update

   // calculate the revolutions per minute
   speed = revolutions;

   speed = speed * 1000;
   speed = speed / (float)(millis() - timeold);
   speed = speed * 60 / 7;
   
   timeold = millis();
   revolutions = 0; 

   Serial.print("RPM:");
   Serial.println(speed);
 }
}

