#define NUM_COLL 13
#define NUM_ROW 4
#define NUM_PINS NUM_ROW+NUM_COLL
#define NUM_CUSTOM_MODS 4
#define CUSTOM_MOD_KEY_POS 42, 43, 46, 47
#define NUM_KEYS NUM_COLL*NUM_ROW
#include <Arduino.h>


//arduino begin loop functions
void startup_routine(); //sets up the pins in the arduino
void _begin(); //serial?



//Keyboard functionality related functions

void matrix_scan(); //handles matrix scan
void key_check(int); //checks each key for keypress
void key_sender(); //genereates hid descriptor and calls write_buffer
void custom_mod_handler();//defines which map to use on that cycle and check for custom mod key toggle


//Lower level USB stuff and hex HID codes manipulation

//splits the 2 bytes data into the modifier and key bytes
uint8_t parse_mod_byte(int key); 
uint8_t parse_hidkey_byte(int key);
//receives key, parses it into the mods and key and adds or removes
// it to/from the 8 byte structure
int add_to_buffer(int key);
int remove_from_buffer(int key);

void flush();//zero out buffer
void write_buffer();//sends buffer to host
