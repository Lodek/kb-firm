#include "keyboard.h"
#include "start.h"

key matrix[NUM_KEYS]; //data structure with an array of key objects
char *mapping[NUM_LAYERS][NUM_KEYS] = {{L0},{L1}}; //user defined mappings for each key
int layers[NUM_LAYERS] = {LAYER_VALUE}; //array that makes the matching of index number of layer with the value utilized by layer var

int layervar = 0; //int that stores which layer is being used
int translated_layervar=0; //layervar after going through translated function. Used as index for key.data[translated_layervar] calls

int index;//int used to iterate through matrix

uint8_t out_buffer[8] = {0}; //buffer to output USB data

//physical pin number for each row and collunm
int row_pins[NUM_ROW] = {ROW_PINS};
int col_pins[NUM_COLL] = {COL_PINS};


void start()
{
	start_gpio();
	start_keys();
	start_mapping();
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

    if (current_key->data[translated_layervar].behavior == normal) normal_behavior(current_key); //normal key
	else if (current_key->data[translated_layervar].behavior==clear) clear_behavior(current_key); //clear key
	else if (current_key->data[translated_layervar].behavior==hold) hold_behavior(current_key); //hold key
	else if (current_key->data[translated_layervar].behavior==doubletap) doubletap_behavior(current_key);//double tap
	else if (current_key->data[translated_layervar].behavior==dtaphold) dtaphold_behavior(current_key);//double tap + hold
    return ;
}


void write_buffer()
{
	//Sends the USB report to the host, the data element of the report is out_buffer which has length 8
    HID_SendReport(2, out_buffer, 8);
    return;
}


//Behavior Functions
void normal_behavior (key *current_key)
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
  	hold_data *data = (hold_data *) current_key->data[translated_layervar].keycode;

	delay(HOLD_SLEEP);
	get_status(current_key);
	if(current_key->state == 0) add_to_buffer(current_key, data->tap); 
	else if(current_key->state==1) add_to_buffer(current_key, data->hold);
	return;
}


void doubletap_behavior(key *current_key)
{
	doubletap_data *data = (doubletap_data *) current_key->data[translated_layervar].keycode;
	
	delay(DOUBLETAP_RELEASE_DELAY);
	get_status(current_key);
	if(current_key->state == 1)
	{
		add_to_buffer(current_key, data->tap);
		return;
	}
	else 
	{
		delay(DOUBLETAP_TAP_DELAY);
		get_status(current_key);
		if(current_key->state == 0)
		{
			add_to_buffer(current_key, data->tap);
			return;
		}
		else
		{
			add_to_buffer(current_key, data->doubletap);
			return; 
		}
	}
}


void clear_behavior(key *current_key)
{
	long data = current_key->data[0].keycode;
	add_to_buffer(current_key, data);
}


void dtaphold_behavior(key *current_key)
{
	dtaphold_data *data = (dtaphold_data *) current_key->data[translated_layervar].keycode;

	delay(DTAPHOLD_RELEASE_DELAY);
	get_status(current_key);
	if(current_key->state == 1)
	{
		add_to_buffer(current_key, data->hold);
		return;
	}
	else 
	{
		delay(DTAPHOLD_TAP_DELAY);
		get_status(current_key);
		if(current_key->state == 0)
		{
			add_to_buffer(current_key, data->tap);
			return;
		}
		else
		{
			add_to_buffer(current_key, data->doubletap);
			return; 
		}
	}	
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
    for (int i = 0; i < NUM_LAYERS ; i++) if (layers[i] == layervar) translated_layervar=i;
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
