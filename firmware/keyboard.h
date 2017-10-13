#include "defines.h"
#include <Arduino.h>

typedef enum {normal, hold, doubletap, clear, dtaphold, macro, dead} key_behavior; //possible behaviors of a key


typedef struct
{
	//struct that stores the information of a key for a layer

    key_behavior behavior;	//defines the behavior of said key
    uint8_t index; //index of the key in the array with keycodes
} key_data;


typedef struct
{
	/* definition of a key
	contains state variables and an array of type key_data of length equal to the number of layers  */

	uint8_t state; // state of the key (1/0) from the time the matrix was scanned
    uint8_t important; //ON/OFF is not enough to track a key, this allows the code to decide whether this key requires attention or not

	/* buffer_value has value of -1 if the key is not on the buffer/is not being used for layer
	otherwise it follows a similar format to the keycode variable: 0xTTLLMMBB
	TT is the toggle/tap byte, LL is the layer byte,  MM HID mod byte
	BB carries the position of hat key on the out_buffer array */
    long buffer_value;
    
	key_data data[NUM_LAYERS]; //array with behaviors and index position for each layer
  
} key;



//Loop functions
void matrix_scan();//performs the scan of the matrix and writes 1/0 to each key
void matrix_iterator();//iterates through each key and calls the handler function
void key_handler(key* current_key);//calls the appropriate handler for that key
void write_buffer();//sends buffer to host

//Behavior functions
void normal_behavior(key* current_key); //handles "normal" keys
void hold_behavior(key* current_key); //handler for hold type keys
void doubletap_behavior(key* current_key);//handler for  double tap type keys
void clear_behavior(key* current_key);//handler for clear keys
void dtaphold_behavior(key* current_key);//handler for double tap + hold keys

//Utility functions
void add_to_buffer(key* current_key, long key);//receives a key object and the keycode that is to be added to the buffer
void remove_from_buffer(key* current_key);//receives a key object, uses buffer_value variable to do the key removal
void flush();//erases the out buffer
void layervar_translator(); //function that returns the index for the corresponding VALUE of a layer.
void get_status(key *current_key);//Performs digitalRead for the current key

extern key matrix[NUM_KEYS];
extern  uint8_t row_pins[NUM_ROW];
extern  uint8_t col_pins[NUM_COLL];
