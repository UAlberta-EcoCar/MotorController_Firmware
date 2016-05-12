#include <Arduino.h>

volatile byte revolutions;

unsigned int rpmilli;
float speed;

int encoder_pin = 16;
int encoder_pin2 = 17;
int encoder_pin3 = 15;

unsigned long timeold;
float WHEELCIRC;

int radius = 0.444;

void setup()
{
 Serial.begin(9600);
 Serial.print("Serial Open");
 Serial.print("Attach Interrupt");
 attachInterrupt(encoder_pin, rpm_fun, RISING);
 Serial.print("Fuck Yea");
	pinMode(encoder_pin3, INPUT);
	pinMode(encoder_pin2, INPUT);
 revolutions = 0;
 rpmilli = 0;
 timeold = 0;
}

void loop()
{
 Serial.print("Encoder Raw Data: ");
 Serial.println(digitalRead(encoder_pin));
 Serial.print("Encoder 2 Raw Data: ");
 Serial.println(digitalRead(encoder_pin2));
 Serial.print("Encoder 3 Raw Data: ");
 Serial.println(digitalRead(encoder_pin3));
 if (revolutions >= 20) {
   //Update RPM every 20 counts, increase this for better RPM resolution,
   //decrease for faster update

   // calculate the revolutions per milli(second)
 rpmilli = revolutions/(millis() - timeold);

    timeold = millis();
    revolutions = 0; //(EDIT: revolutions = 0;)

    WHEELCIRC = 2 * PI * radius;
//   speed = rpmilli * WHEELCIRC * "milliseconds per hour" / "meters per kilometer"

   // simplify the equation to reduce the number of floating point operations
   // speed = rpmilli * WHEELCIRC * 3600000 / 1000
   // speed = rpmilli * WHEELCIRC * 3600

   speed = rpmilli * WHEELCIRC * 3600;

   Serial.print("RPM:");
   Serial.print(rpmilli * 60000,DEC);
   Serial.print(" Speed:");
   Serial.print(speed,DEC);
   Serial.println(" kph");
 }
}

void rpm_fun()
{
 revolutions++;
}
