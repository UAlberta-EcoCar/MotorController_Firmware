//code for using an arduino to dyno test a motor

//compiler directives to speed up interrupts and save ram
//#define EI_NOTEXRERNAL  //uncomment if you are not using external interrupts
//#define EI_NOTINT0 //or this if you are only using int1
//#define EI_NOTINT1 //or this if you are only using int0
//#define EI_NOTPINCHANGE //uncomment if you are using pin change interrupts
//#define EI_NOTPORTB // uncomment if no port B interrupts are used
//#define EI_NOTPORTC // same for port C
//#define EI_NOTPORTD // port D

#include <EnableInterrupt.h>

//define the hall effect sensor pins
#define hall_1 1
#define hall_2 2
#define hall_3 3

//define curent input pin
#define current_pin A0

//make variables for storing data
uint16_t pulse_count = 0;
uint16_t last_pulse_count = 0;
uint16_t motor_current = 0;

//make variable for timing writing to serial port
uint32_t time_var = 0;

//make function for counting pulses
void count_pulses (void)
{
  pulse_count++;
}

void setup() {
  Serial.begin(115200);

  pinMode(hall_1,INPUT);
  pinMode(hall_2,INPUT);
  pinMode(hall_3,INPUT);

  pinMode(current_pin,INPUT);

  //set up interrupts
  enableInterrupt(hall_1,count_pulses,RISING); //every time hall_1 pin rises count_pulses function wil run
  enableInterrupt(hall_2,count_pulses,RISING);
  enableInterrupt(hall_3,count_pulses,RISING);

  //reset time
  time_var = millis();
}



void loop() {
  if(millis() - time_var > 50) //every 50 millis
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
}
