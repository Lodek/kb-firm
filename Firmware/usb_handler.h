#include <Arduino.h>

uint8_t out_buffer[8] = 0;

void begin(); //starts comms with 16U2
void write(); //writes the 8 byte data structure with serial.write

//splits the 2 bytes with info into the modifier and key bytes
uint8_t parse_mod_byte(int key); 
uint8_t parse_hidkey_byte(int key);

//receives key, parses it into the mods and key and adds or removes
// it to/from the 8 byte structure
int add_to_buffer(int key);
int remove_from_buffer(int key);
