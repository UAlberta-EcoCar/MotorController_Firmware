// #include "esc_can.h"
#include "esc.h"
#include "sensors.h"
#include "hardware.h"
#include <FlexCAN.h>

int pin = 7;

//Create objects for classes
Esc myEsc;

void setup() {
	Serial.begin(BR);
  myEsc.beginTest();
  pinMode(pin, INPUT); // Initialize Encoder pins
}

//int test=0;
void loop() {

	myEsc.write(10);
	delay(10000);
	myEsc.write(0);
	while(true){}
	/*
	Serial.print("Pushed? ");
	Serial.println(digitalRead(pin));
	if (digitalRead(pin) == HIGH){
		myEsc.write(180);
	}
	else{
		myEsc.write(0);
	}
	delay(50);
*/

}
