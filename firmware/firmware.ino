#include "Keyboard.h"

void setup() 
{
  startup_routine();
  init_mappings();
}

void loop() 
{
    matrix_scan();
    matrix_iterator();
    write_buffer();
}

