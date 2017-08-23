#include "Keyboard.h"

void setup() 
{
  startup_routine();
}

void loop() 
{
    matrix_scan();
    matrix_iterator();
    write_buffer();
}

