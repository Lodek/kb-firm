#include "defines.h"
typedef enum {normal, hold, doubletap, clear, dtaphold, macro, dead} key_behavior; //possible behaviors of a key

typedef struct
{
	long tap;
	long hold;
} hold_data;


typedef struct
{
	long tap;
	long doubletap;
} doubletap_data;


typedef struct
{
	long tap;
	long doubletap;
	long hold;
} dtaphold_data;


typedef struct
{
	//struct that stores the information of a key for a layer
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
	/* buffer_value has value of -1 if the key is not on the buffer/is not being used for layer
	otherwise it follows a similar format to the keycode variable: 0xTTLLMMBB
	TT is the toggle/tap byte, LL is the layer byte,  MM HID mod byte
	BB carries the position of hat key on the out_buffer array */
    long buffer_value;
    
	key_data data[NUM_LAYERS]; //array with the keycode and the behavior/function of this key for each layer
  
} key;

