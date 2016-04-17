#ifndef ESC_STARTUP_H
#define ESC_STARTUP_H

#include <Arduino.h>
#include <Servo.h>
#include "hardware.h"

class Esc {
public:

  Esc() {};
  void begin();

private:
  Servo * _esc; // We created a variable that can store the address of the servo object (THE OBJECT IS NOT _ESC)

};
#endif
