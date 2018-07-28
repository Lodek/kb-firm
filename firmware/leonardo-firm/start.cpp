#include "start.h"
#include <Arduino.h>
#include "defines.h"
#include "mappings.h"
#include "keyboard.h"

const static uint8_t  behaviors[NUM_LAYERS][NUM_KEYS] PROGMEM = {BEHAVIORS} ;
const static uint8_t  indexes[NUM_LAYERS][NUM_KEYS] PROGMEM = {INDEXES} ;

void start_serial()
{
	/* begins commns serial communication
	no longer needed for pro micro, still used for uno and mega */
    Serial.end();
    Serial.begin(9600);
}


void start_gpio()
{
	/* initialize microcontroller
	sets rows as outputs and colls as inputs
	begins serial communication */
    for (uint8_t i = 0; i < NUM_ROW; i++) //sets row pins as OUT
    {
        pinMode(row_pins[i], OUTPUT);
        digitalWrite(row_pins[i], LOW);
    }
    for (uint8_t i = 0; i < NUM_COLL; i++) //sets colls pins as IN
        pinMode(col_pins[i], INPUT);
    return;
}


void start_keys()
{
	for (uint8_t i = 0; i < NUM_KEYS; i++)
    {
		 matrix[i].important = 0;
         matrix[i].buffer_value = -1;
         matrix[i].state = 0;
         for (uint8_t j = 0; j < NUM_LAYERS; j++)
         {
            matrix[i].data[j].behavior = (key_behavior) pgm_read_byte_near(&behaviors[j][i]);
			matrix[i].data[j].index = pgm_read_byte_near(&indexes[j][i]);
         }
    }
    return;
}
