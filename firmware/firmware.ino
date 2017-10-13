#include "defines.h"
#include "keyboard.h"
#include "start.h"

//Global variables

key matrix[NUM_KEYS]; //data structure with an array of key objects

//physical pin number for each row and collunm
uint8_t row_pins[NUM_ROW] = {ROW_PINS};
uint8_t col_pins[NUM_COLL] = {COL_PINS};


void setup() 
{
//	start_serial();
	start_gpio();
	start_keys();
}

void loop() 
{
    matrix_scan();
    matrix_iterator();
    write_buffer();
}

