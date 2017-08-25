#define NUM_COLL 13
#define NUM_ROW 4
#define NUM_PINS NUM_ROW+NUM_COLL
#define NUM_KEYS NUM_COLL*NUM_ROW
#define NUMLAYERS 3
#define LAYERVALUE 0,1,2
#include <Arduino.h>

//possible behaviors of a key
typedef enum
{normal, hold, doubletap, clear, macro, dth, dead} behavior_enum; 

//struct that stores the information of each key for each layer
typedef struct
{
	//defines the behavior of said key
    behavior_enum behavior;

    //contains either a pointer to a struct of type hold, doubletap...
	//or carries the keycode on the format 0xLLMMKK
	//where LL the byte for layer selection
	//MM is the byte for HID modifiers
	//KK is the byte for HID key  
    long keycode; 
} key_data;

//actual definition of a key
//contains state variables and an array of type key_data
//the array carries the data/behavior of the key for each layer defined by the user
typedef struct
{
	int state; // state of the key (1/0) from the time the matrix was scanned
    int important; //ON/OFF is not enough to track a key, this allows the code to decide whether this key requires attention or not
    long on_buffer;
	// on buffer has value of -1 if the key is not on the buffer/is not being used for layer
	//otherwise it follows a similar format to the keycode variable
	//0xLLMMBB
	//LL is the layer byte
	// MM HID mod byte
	// BB is the difference, it instead of containing the HID keycode, it carries the position of that key on the out_buffer array
    
	key_data data[NUMLAYERS]; //array with the data of the behavior/function of this key for each layer
  
} key;

void startup_routine(); //sets up the pins in the arduino
void _begin(); //serial?
void init_mappings();
void matrix_scan();
void matrix_iterator();
void key_handler(key* current_key);
void behavior_normal(key* current_key);
void flush();//zero out buffer
void write_buffer();//sends buffer to host

int layervar_translator(int layer);
int add_to_buffer(long key);
int remove_from_buffer(long key);
