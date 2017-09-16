#include "keyboard.h"

void setup() 
{
  start();
}

void loop() 
{
    matrix_scan();
    matrix_iterator();
    write_buffer();
}

