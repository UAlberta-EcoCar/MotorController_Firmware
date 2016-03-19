//code for using an arduino to dyno test a motor

//compiler directives to speed up interrupts and save ram
//#define EI_NOTEXRERNAL  //uncomment if you are not using external interrupts
//#define EI_NOTINT0 //or this if you are only using int1
//#define EI_NOTINT1 //or this if you are only using int0
//#define EI_NOTPINCHANGE //uncomment if you are using pin change interrupts
//#define EI_NOTPORTB // uncomment if no port B interrupts are used
//#define EI_NOTPORTC // same for port C
//#define EI_NOTPORTD // port D

#define NEEDFORSPEED
#define INTERRUPT_FLAG_PIN3 pulse_count

#include <EnableInterrupt.h>
#include <Servo.h>

//define the hall effect sensor pin
#define hall_pin 3 //int1

//define curr ent input pin
#define current_pin A0

//define esc pwm pin
#define servo_pin 10

//make variables for storing data
uint16_t last_pulse_count = 0;
uint16_t motor_current = 0;

//make variable for timing writing to serial port
uint32_t time_var = 0;
uint32_t time_car = 0;

Servo esc; //create servo object

void setup() {
  Serial.begin(115200);

  pinMode(hall_pin,INPUT);

  pinMode(current_pin,INPUT);

  enableInterruptFast(hall_pin, RISING);

  esc.attach(servo_pin);
  //ESC needs a low high low signal to turn on
  esc.write(0);
  delay(1000);
  esc.write(180);
  delay(500);
  esc.write(0);
  delay(500);

  //give person 10 seconds to run away
  delay(1000);

  //datalogger starts when a $ is sent
  Serial.print("$");

  //reset pulse count
  pulse_count = 0;
  
  //reset time
  time_var = millis();
  time_car = millis();

  //start motor
  esc.write(50);
}



void loop() {
  if(millis() - time_var > 100) //every 100 millis
  {
    //record pulse count
    last_pulse_count = pulse_count;

    //reset pulse count
    pulse_count = 0;
    
    //reset time 
    time_var = millis();
    
    //record current
    motor_current = analogRead(current_pin);

    //send data
    Serial.print(motor_current);
    Serial.print(",");
    Serial.println(last_pulse_count);
  }
  if (millis() - time_car > 40000)
  {
    esc.write(0);
  }
  if(millis() - time_car > 80000)
  {
    while(1);
  }
}

