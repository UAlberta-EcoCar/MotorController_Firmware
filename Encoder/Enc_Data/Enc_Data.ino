#define PIN_A 11
#define PIN_B 12
#define PIN_C 13

#define DELAY 100

#include "Timer.h"
int array = 1000;//Pick size of data array

int d[array];

//int e;// to be used with Hall Effect sensor three
//int f;// to be used with Hall Effect sensor three

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_A, INPUT);
  pinMode(PIN_B, INPUT);
  pinMode(PIN_C, INPUT);
  Serial.begin(9600);
  uint16_t t = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  int a = digitalRead(PIN_A);
  int b = digitalRead(PIN_B);
  int c = digitalRead(PIN_C);

  // finding velocity
  int i;
  for (i = 0, i < array, i++) {
    if (a=0) {
      d[i]=t
    }
      
  }
  
  Serial.print(a);
  Serial.print(",");
  Serial.print(b);
  Serial.print(",");
  Serial.println(c);
  //delay(DELAY);   
}
