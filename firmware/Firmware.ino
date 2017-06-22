#include "Keyboard.h"

void setup() 
{
  startup_routine();
  
}

void loop() 
{
  matrix_scan();
  custom_mod_handler();
  key_sender();
}

