#include "Keyboard.h"

//data structure with an array of key objects
//it contains all information regarding the keyboard
key matrix[NUM_KEYS]; 

//int that stores which layer is being used
int layervar = 0;
int translated_layervar=0;
int index;
//buffer to output USB data
uint8_t out_buffer[8] = {0};

//physical pin number for each row and collunm
int row_pins[NUM_ROW] = {1, 21, 2, 3};
int col_pins[NUM_COLL] = {4, 5, 6, 7, 8, 9, 10, 16, 20, 15, 19, 14, 18};

//array that makes the matching of index number of layer with the value utilized by layer var
int layers[NUMLAYERS] = {LAYERVALUE};

//user defined mappings for each key
long mapping[NUMLAYERS][NUM_KEYS] =
{
	{0x0100002b, 0x0014, 0x001a, 0x0008, 0x0015, 0x0017, 0x001c, 0x0018, 0x000c, 0x0012, 0x0013, 0x002a, 0x002a, 0x0029, 0x0004, 0x0016, 0x0007, 0x0009, 0x000a, 0x000b, 0x000d, 0x000e, 0x000f, 0x0033, 0x0034, 0x0028, 0x0200, 0x001d, 0x001b, 0x0006, 0x0019, 0x0005, 0x0011, 0x0010, 0x0036, 0x0037, 0x0038, 0x2000, 0x0028, 0x0100, 0x0800, 0x0400, 0x0000, 0x010000, 0x002c, 0x002c, 0x020000, 0x0000, 0x0000, 0x0000, 0x1000, 0x004c},
	{0x0037, 0x0024, 0x0025, 0x0026, 0x2024, 0x2025, 0x002d, 0x0031, 0x0038, 0x2026, 0x2027, 0x002a, 0x002a, 0x0027, 0x0021, 0x0022, 0x0023, 0x2021, 0x2022, 0x2023, 0x2036, 0x2037, 0x202f, 0x2030, 0x2031, 0x0028, 0x0200, 0x001e, 0x001f, 0x0020, 0x201e, 0x201f, 0x2020, 0x002e, 0x0035, 0x002f, 0x0030, 0x2000, 0x0028, 0x0100, 0x0800, 0x0400, 0x0000, 0x010000, 0x002a, 0x002a, 0x020000, 0x0000, 0x0000, 0x0000, 0x1000, 0x002c},
	{0x003a, 0x003b, 0x003c, 0x003d, 0x003e, 0x003f, 0x0040, 0x0041, 0x0042, 0x0043, 0x0044, 0x002a, 0x002a, 0x0039, 0x007f, 0x0081, 0x0080, 0x0074, 0x0078, 0x0050, 0x0051, 0x0052, 0x004f, 0x0048, 0x0045, 0x0028, 0x0200, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x004a, 0x004e, 0x004b, 0x004d, 0x0046, 0x2000, 0x0028, 0x0100, 0x0800, 0x0400, 0x0000, 0x010000, 0x0028, 0x0028, 0x020000, 0x0000, 0x0000, 0x0000, 0x1000, 0x002c},
};

// Initialize functions
void init_mappings()
{
	for (int i = 0; i < NUM_KEYS; i++)
    {
		 matrix[i].important = 0;
         matrix[i].buffer_value = -1;
         matrix[i].state = 0;
         for (int j = 0; j < NUMLAYERS; j++)
         {
            matrix[i].data[j].behavior = normal;
			matrix[i].data[j].keycode = mapping[j][i];
         }
    }
    return;
}


void _begin()
{
	/* begins commns serial communication
	no longer needed for pro micro, still used for uno and mega */
    Serial.end();
    Serial.begin(9600);
}


void startup_routine()
{
	/* initialize microcontroller
	sets rows as outputs and colls as inputs
	begins serial communication */
    for (int i = 0; i < NUM_ROW; i++) //sets row pins as OUT
    {
        pinMode(row_pins[i], OUTPUT);
        digitalWrite(row_pins[i], LOW);
    }
    for (int i = 0; i < NUM_COLL; i++) //sets colls pins as IN
        pinMode(col_pins[i], INPUT);
    _begin();
    init_mappings();
	
    return;
}



// Loop functions


void matrix_scan()
{
	/* uses col_pins & row_pins
	loops through each row; sets it to high, calls digital read for each coll_pin member and assigns it to the matching key object
	writes result of digital read to the matching key on the matrix array
	resets current row to low
	repeats for next row */
    for (int i = 0; i < NUM_ROW; i++)
    {
        digitalWrite(row_pins[i], HIGH);
		for (int j = 0; j < NUM_COLL; j++)  matrix[i*NUM_COLL+j].state = digitalRead(col_pins[j]);
		digitalWrite(row_pins[i], LOW);
    }
    return;
}


void matrix_iterator()
{
	//calls layervar_translator and then calls key handler for each element of  matrix[]
	layervar_translator();
    for (index = 0; index < NUM_COLL * NUM_ROW; index++)
    {
		key_handler(&matrix[index]);
    }
    return;
}


void key_handler(key *current_key)
{
	/* key handler receives a key struct object
	it first checks whether the key needs to be handled
	then it checks the key behavior and sends calls to appropriate handler function for that key type */
    if(current_key->state == 0 && current_key->important == 0) return; // key is neither pressed nor important
	if(current_key->state == 1 && current_key->important == 1) return; //trivial, key is both pressed and on buffer

	if(current_key->state == 0 && current_key->important == 1) 
	{
		remove_from_buffer(current_key); //key has been released and is still important
		return;
	}

    if (current_key->data[translated_layervar].behavior == normal) behavior_normal(current_key); //normal key
	else if (current_key->data[translated_layervar].behavior==hold) hold_behavior(current_key); //hold key
    return ;
}


void write_buffer()
{
	//Sends the USB report to the host, the data element of the report is out_buffer which has length 8
    HID_SendReport(2, out_buffer, 8);
    return;
}


//Behavior Functions

void behavior_normal (key *current_key)
{
	// Simple handler for a normal key. If key is pressed add it to buffer
	add_to_buffer(current_key, current_key->data[translated_layervar].keycode); //calls add to buffer and writes the return to buffer_value variable

    return;
}


void hold_behavior(key* current_key)
{
	/* Handler for hold behavior
	First make a hold_struct pointer variable and initializes it with the address of the struct that contains the data
	sleep the MCU for HOLD SLEEP ms
	checks, if key still pressed, add to buffer using the hold keycode
	else uses the tap keycode */
  	hold_struct *key_data = (hold_struct *) current_key->data[translated_layervar].keycode;

	delay(HOLD_SLEEP);
	get_status(current_key);
	if(current_key->state == 0) add_to_buffer(current_key, key_data->tap); 
	else if(current_key->state==1) add_to_buffer(current_key, key_data->hold);
	return;
}


//Utility functions

void add_to_buffer(key* current_key, long key)
{
	/* Receives a key and does 3 operations. Returns the index of out_buffer where the HID Keycode was written to. If buffer is full, returns -1.
	It checks whether the buffer_value is not -1. If that is the case the key is toggle is turned on and it's on buffer still.
	If the toggle is on, it turns it off and returns, the following is the normal behavior
	a - isolates and adds HID Keycode part of key to out_buffer (first byte of key, bitshifts and ANDs to isolate it).
	b - isolates and appends (ORs) HID modifier to out_buffer[0] (second byte of key).
	c - isolates and appends (ORs) layer byte of key to layervar (third byte of key) */
	if(current_key->buffer_value!=-1) //toggle is turned on
	{
		current_key->important=1;
		current_key->buffer_value = current_key->buffer_value &(0x00FFFFFF);
		return; 
	}

	//In case key's 1st byte is 0, return 1. 1 is the reserved byte of the HID descriptor
	//Needed since otherwise 2 keys might point to the same index of out_buffer. Then releasing a key would release both and lead to an awkward situation
	if((key&0x000000FF)==0) current_key->buffer_value=1; 
	else
	{
	    for (int k = 2; k < 8; k++) //loops through out buffer looking for the first empty spot
	    {
	        if (out_buffer[k] == 0)
	        {
				out_buffer[k] = (key&0x000000FF); //a
				current_key->buffer_value = k;
				k = 9;
			}
	    }
	}
	if(current_key->buffer_value==-1) //buffer was full and nothing was written
	{
		current_key->important=0;
		return;
	}
    out_buffer[0] = out_buffer[0] | ((key >> 8) & 0x000000FF); //b 
    layervar = ((key >> 16) & 0x000000FF) | layervar; //c 
	current_key->important=1;
	current_key->buffer_value=(key&0xFFFFFF00) | current_key->buffer_value;
    return ;
}


void remove_from_buffer(key *current_key)
{
	/*checks for a toggle then removes the received key from the buffer and removes its value from layer var
	first checks the 4th byte for a true. If true, toggle is on, important=0 and return
   	3 removal steps
	a - Removes key from buffer. Isolates first byte of key which contains the position of that key on out_buffer
	b - Removes modfier from buffer. Isolates second byte of key. That byte contais HID Mod code. Uses the data and XOR's it with the modifier byte of out_buffer
	c - Removes layer value from layervar. Isolates third byte of key which has layer value. Similarly, XOR's it with layervar to subtract its value */
	if(((current_key->buffer_value>>24) & 0x000000FF)==1)
	{
		current_key->important=0;
		return;
	}
    out_buffer[(current_key->buffer_value & 0x000000FF)] = 0; //a
	out_buffer[0] = out_buffer[0] ^ ((current_key->buffer_value >> 8) & 0x000000FF);//b
    layervar = ((current_key->buffer_value >> 16) &0x000000FF)^layervar;//c

	current_key->buffer_value=-1;
	current_key->important=0;
    return;
}


void flush()
{
	//loops through out_buffer zeroing all elements of the array
    for (int i = 0; i < 8; i++) out_buffer[i] = 0;
    return;
}


void layervar_translator()
{
	//function that returns the index for the corresponding VALUE of a layer (1,2,4,8...)
    for (int i = 0; i < NUMLAYERS ; i++) if (layers[i] == layervar) translated_layervar=i;
	return;
}


void get_status(key *current_key)
{
	// uses index variable and checks the status of the current key
	digitalWrite(row_pins[index / NUM_COLL], HIGH);
	current_key->state = digitalRead(col_pins[index % NUM_COLL]);
	digitalWrite(row_pins[index / NUM_COLL], LOW);
	return;
}
