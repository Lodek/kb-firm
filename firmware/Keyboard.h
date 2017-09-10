#include "behavior_struct.h"
#include <Arduino.h>
#define NUM_COLL 13
#define NUM_ROW 4
#define NUM_PINS NUM_ROW+NUM_COLL
#define NUM_KEYS NUM_COLL*NUM_ROW
#define NUMLAYERS 3
#define LAYERVALUE 0,1,2

#define HOLD_SLEEP 150
#define DOUBLETAP_TAP_DELAY  100
#define DOUBLETAP_RELEASE_DELAY 100
#define DTAPHOLD_RELEASE_DELAY 100
#define DTAPHOLD_TAP_DELAY  100

typedef enum {normal, hold, doubletap, clear, macro, dth, dead} key_behavior; //possible behaviors of a key

typedef struct
{
	//struct that stores the information of each key for each layer
    key_behavior behavior;	//defines the behavior of said key

    /*contains either a pointer to a struct with the keycode data for "special" keys
	or carries the keycode on the format 0xTTLLMMKK
	where TT defines whether a key is of type toggle or not
	LL the byte for layer selection
	MM is the byte for HID modifiers
	KK is the byte for HID key  */
    long keycode; 
} key_data;


typedef struct
{
	/* actual definition of a key
	contains state variables and an array of type key_data
	the array carries the data/behavior of the key for each layer defined by the user */
	int state; // state of the key (1/0) from the time the matrix was scanned
    int important; //ON/OFF is not enough to track a key, this allows the code to decide whether this key requires attention or not
	/* on buffer has value of -1 if the key is not on the buffer/is not being used for layer
	otherwise it follows a similar format to the keycode variable: 0xTTLLMMBB
	TT is the toggle/tap byte, LL is the layer byte,  MM HID mod byte
	BB carries the position of hat key on the out_buffer array */
    long buffer_value;
    
	key_data data[NUMLAYERS]; //array with the keycode and the behavior/function of this key for each layer
  
} key;

//Startup functions
void init_mappings();//maps the mapping array to the key structs
void _begin(); //init serial comms
void startup_routine(); //sets up the pins in the arduino

//Loop functions
void matrix_scan();//performs the scan of the matrix and writes 1/0 to each key
void matrix_iterator();//iterates through each key and calls the handler function
void key_handler(key* current_key);//calls the appropriate handler for that key
void write_buffer();//sends buffer to host

//Behavior functions
void normal_behavior(key* current_key); //handles "normal" keys
void hold_behavior(key* current_key); //handler for hold type keys
void doubletap_behavior(key* current_key);//handler for  double tap type keys
void clear_behavior(key* current_key);
void dtaphold_behavior(key* current_key);

//Utility functions
void add_to_buffer(key* current_key, long key);//receives a key object and the keycode that is to be added to the buffer
void remove_from_buffer(key* current_key);//receives a key object, uses buffer_value variable to do the key removal
void flush();//erases the out buffer
void layervar_translator(); //function that returns the index for the corresponding VALUE of a layer.
void get_status(key *current_key);
