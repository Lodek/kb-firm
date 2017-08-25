#include <Arduino.h>
#define NUM_COLL 13
#define NUM_ROW 4
#define NUM_PINS NUM_ROW+NUM_COLL
#define NUM_KEYS NUM_COLL*NUM_ROW
#define NUMLAYERS 3
#define LAYERVALUE 0,1,2


//possible behaviors of a key
typedef enum {normal, hold, doubletap, clear, macro, dth, dead} behavior_enum; 


typedef struct
{
	//struct that stores the information of each key for each layer
    behavior_enum behavior;	//defines the behavior of said key

    /*contains either a pointer to a struct of type hold, doubletap...
	or carries the keycode on the format 0xLLMMKK
	where LL the byte for layer selection
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
	otherwise it follows a similar format to the keycode variable: 0xLLMMBB
	LL is the layer byte,  MM HID mod byte
	BB carries the position of that key on the out_buffer array */
    long on_buffer;
    
	key_data data[NUMLAYERS]; //array with the data of the behavior/function of this key for each layer
  
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
void behavior_normal(key* current_key); //handles "normal" keys

//Utility functions
int add_to_buffer(long key);//receives a standard key, parses it and add to buffer
int remove_from_buffer(long key);//receives std key, parses it and erases it from buffer
void flush();//zero out buffer
int layervar_translator(int layer); //function that returns the index for the corresponding VALUE of a layer.
