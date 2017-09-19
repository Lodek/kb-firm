#include "start.h"
#include "keyboard.h"
#include <Arduino.h>
#include "defines.h"

//const char *mappings[NUM_LAYERS][NUM_KEYS] = {{L0},{L1}};

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
            matrix[i].data[j].behavior = clear;
			matrix[i].data[j].keycode = 0;
         }
    }
    return;
}

void start_layer(char **map, int current_layer)
{
		for(uint8_t current_key=0; current_key<NUM_KEYS; current_key++)
		{
			char *keycode_str =  map[current_key];

			int behavior;//long that hold behavior of current key
			char behavior_str[2];//behavior string parsed from mapping
			behavior_str[0] = keycode_str[0];
			behavior_str[1] = '\0';
			sscanf(behavior_str, "%d", &behavior); //sscanf behavior str to get its num value
			keycode_str = keycode_str+1; //moves pointer of keycode to the next block of 10 characters

			if(behavior==normal)//early termination for normal key
			{
				matrix[current_key].data[current_layer].behavior=normal;
				long normal_keycode;
				sscanf(keycode_str, "%lx", &normal_keycode);
				matrix[current_key].data[current_layer].keycode=normal_keycode;
			}
			else if(behavior==clear) //early termination for clear key
			{
				matrix[current_key].data[current_layer].behavior=clear;
				matrix[current_key].data[current_layer].keycode=0;
			}
			else
			{
				char keycodes[10][9];//matrix with parsed keycodes for current key			
				uint8_t keycodes_len=strlen(keycode_str);//num of characters in the string 
				//parses rest of the mapping string. writes each 10 character block into a different string for sscanf usage
				for(uint8_t i=0; i<keycodes_len/8; i++)
				{
					for(uint8_t j=0; j<8; j++)
					{
						keycodes[i][j]=keycode_str[i*8+j];
					}
					keycodes[i][8]='\0';
				}
				
				switch(behavior)//dynamically allocate structs if needed
				{
					case hold:
					hold_data *hold_keycode;
					hold_keycode = (hold_data *) malloc (sizeof(hold_data));
					sscanf(keycodes[0], "%lx", &hold_keycode->tap);
					sscanf(keycodes[1], "%lx", &hold_keycode->hold);
					matrix[current_key].data[current_layer].keycode= (long) hold_keycode;
					matrix[current_key].data[current_layer].behavior = hold;
					break;
			
					case doubletap:
					doubletap_data *doubletap_keycode;
					doubletap_keycode = (doubletap_data *) malloc(sizeof(doubletap_data));
					sscanf(keycodes[0], "%lx", &doubletap_keycode->tap);
					sscanf(keycodes[1], "%lx", &doubletap_keycode->doubletap);
					matrix[current_key].data[current_layer].keycode= (long) doubletap_keycode;
					matrix[current_key].data[current_layer].behavior = doubletap;
					break;
			
					case dtaphold:
					dtaphold_data *dtaphold_keycode;
					dtaphold_keycode = (dtaphold_data *) malloc(sizeof(dtaphold_data));
					sscanf(keycodes[0], "%lx", &dtaphold_keycode->tap);
					sscanf(keycodes[1], "%lx", &dtaphold_keycode->doubletap);
					sscanf(keycodes[2], "%lx", &dtaphold_keycode->hold);
					matrix[current_key].data[current_layer].keycode= (long) dtaphold_keycode;
					matrix[current_key].data[current_layer].behavior = dtaphold;					
					break;
				}
		}
	}
	return;
}

/*
void start_mapping()
{
	for(uint8_t current_layer=0; current_layer<NUM_LAYERS; current_layer++)
	{
		for(uint8_t current_key=0; current_key<NUM_KEYS; current_key++)
		{
			char *keycode_str = (char*) mappings[current_layer][current_key];

			int behavior;//long that hold behavior of current key
			char behavior_str[2];//behavior string parsed from mapping
			behavior_str[0] = keycode_str[0];
			behavior_str[1] = '\0';
			sscanf(behavior_str, "%d", &behavior); //sscanf behavior str to get its num value
			keycode_str = keycode_str+1; //moves pointer of keycode to the next block of 10 characters

			if(behavior==normal)//early termination for normal key
			{
				matrix[current_key].data[current_layer].behavior=normal;
				long normal_keycode;
				sscanf(keycode_str, "%lx", &normal_keycode);
				matrix[current_key].data[current_layer].keycode=normal_keycode;
			}
			else if(behavior==clear) //early termination for clear key
			{
				matrix[current_key].data[current_layer].behavior=clear;
				matrix[current_key].data[current_layer].keycode=0;
			}
			else
			{
				char keycodes[10][9];//matrix with parsed keycodes for current key			
				uint8_t keycodes_len=strlen(keycode_str);//num of characters in the string 
				//parses rest of the mapping string. writes each 10 character block into a different string for sscanf usage
				for(uint8_t i=0; i<keycodes_len/8; i++)
				{
					for(uint8_t j=0; j<8; j++)
					{
						keycodes[i][j]=keycode_str[i*8+j];
					}
					keycodes[i][8]='\0';
				}
				
				switch(behavior)//dynamically allocate structs if needed
				{
					case hold:
					hold_data *hold_keycode;
					hold_keycode = (hold_data *) malloc (sizeof(hold_data));
					sscanf(keycodes[0], "%lx", &hold_keycode->tap);
					sscanf(keycodes[1], "%lx", &hold_keycode->hold);
					matrix[current_key].data[current_layer].keycode= (long) hold_keycode;
					matrix[current_key].data[current_layer].behavior = hold;
					break;
			
					case doubletap:
					doubletap_data *doubletap_keycode;
					doubletap_keycode = (doubletap_data *) malloc(sizeof(doubletap_data));
					sscanf(keycodes[0], "%lx", &doubletap_keycode->tap);
					sscanf(keycodes[1], "%lx", &doubletap_keycode->doubletap);
					matrix[current_key].data[current_layer].keycode= (long) doubletap_keycode;
					matrix[current_key].data[current_layer].behavior = doubletap;
					break;
			
					case dtaphold:
					dtaphold_data *dtaphold_keycode;
					dtaphold_keycode = (dtaphold_data *) malloc(sizeof(dtaphold_data));
					sscanf(keycodes[0], "%lx", &dtaphold_keycode->tap);
					sscanf(keycodes[1], "%lx", &dtaphold_keycode->doubletap);
					sscanf(keycodes[2], "%lx", &dtaphold_keycode->hold);
					matrix[current_key].data[current_layer].keycode= (long) dtaphold_keycode;
					matrix[current_key].data[current_layer].behavior = dtaphold;					
					break;
				}
			}
		}
	}
	return;
}
*/

void start_l0()
{
	char *map[NUM_KEYS] = {L0};
	int current_layer = 0;
	start_layer(map,current_layer);
}

void start_l1()
{
	char *map[NUM_KEYS] = {L1};
	int current_layer = 1;
	start_layer(map,current_layer);
}

