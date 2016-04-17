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
#include "esc_can_handler.h"

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

uint32_t can_bus_write_timer;

Servo esc; //create servo object

void arm_esc(void)
{
  //ESC needs a low high low signal to turn on
  esc.write(0);
  delay(1000);
  esc.write(180);
  delay(500);
  esc.write(0);
  delay(500);
}


#define WHEEL_DIAMETER 444 //mm
#define GEAR_RATIO 26
uint8_t motor_speed;
uint32_t calc_speed_timer;
void calc_speed(uint32_t read_time)
{
    //record pulse count
    last_pulse_count = pulse_count;

    //reset pulse count
    pulse_count = 0;

    //convert to speed
    //7 pulses per motor rotation
    //gear ratio
    //wheel diameter * 3.12mm per wheel rotation
    last_pulse_count = (((((last_pulse_count * WHEEL_DIAMETER * 314) / GEAR_RATIO) / 7) / 10) / 1000);
    //that give meters

    //convert to speed
    last_pulse_count = (last_pulse_count * 1000) / read_time; //meters / sec

    //go to km/hr
    motor_speed = (last_pulse_count * 36) / 10;
}

void setup() {
  Serial.begin(115200);
  
  //start can bus
  esc_can_init();
  Serial.println("CAN BUS STARTED");
  
  pinMode(hall_pin,INPUT);

  pinMode(current_pin,INPUT);

  pinMode(A2,OUTPUT);
  
  enableInterruptFast(hall_pin, RISING);

  esc.attach(servo_pin);
  arm_esc();
  
  //reset pulse count
  pulse_count = 0;
  
  //reset time
  time_var = millis();

  //start motor
  esc.write(50);
}



void loop() {

  
  uint32_t time_dif = millis() - calc_speed_timer;
  if (time_dif > 200)
  {
    calc_speed(time_dif);
    calc_speed_timer = millis();
  }

  if(millis() - can_bus_write_timer > 100)
  {
    digitalWrite(A2,HIGH);
    send_speed(motor_speed);
    can_bus_write_timer = millis();
    digitalWrite(A2,LOW);
  }
}
