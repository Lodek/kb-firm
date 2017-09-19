#include "start.h"
#include "keyboard.h"
#include "defines.h"
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>

using namespace std;

key matrix[NUM_KEYS];
char *mapping[NUM_LAYERS][NUM_KEYS] = {{L1},{L2}};

int main()
{
	start_keys();
	start_mapping();
	return 0;
}

void start_keys()
{
	for (int i = 0; i < NUM_KEYS; i++)
    {
		 matrix[i].important = 0;
         matrix[i].buffer_value = -1;
         matrix[i].state = 0;
         for (int j = 0; j < NUM_LAYERS; j++)
         {
            matrix[i].data[j].behavior = clear;
			matrix[i].data[j].keycode = 0;
         }
    }
    return;
}


void start_mapping()
{
	for(int current_layer=0; current_layer<NUM_LAYERS; current_layer++)
	{
		for(int current_key=0; current_key<NUM_KEYS; current_key++)
		{
			char *keycode_str = mapping[current_layer][current_key] ;//address of the string that contains the data for this key
			char keycodes[20][11];//matrix with parsed keycodes for current key
			long behavior;//long that hold behavior of current key
			char behavior_str[11];//behavior string parsed from mapping
			behavior_str[10]='\0';//sets last character to null, so it's a string
			for(int i=0; i<10; i++) //copies behavior 10 characters into a behavior int
			{
				behavior_str[i]=keycode_str[i];
			}
			sscanf(behavior_str, "%lx", &behavior); //sscanf behavior str to get its num value
			
			keycode_str = keycode_str+10; //moves pointer of keycode to the next block of 10 characters
			
			int keycodes_len=strlen(keycode_str);//num of characters in the string 
			
			cout << behavior_str << "behavior str key " << current_key << " " << current_layer << '\n';
			cout << keycode_str << "keycode str key " << current_key << " " << current_layer << '\n';

			
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
				//parses rest of the mapping string. writes each 10 character block into a different string for sscanf usage
				for(int i=0; i<keycodes_len/10; i++)
				{
					for(int j=0; j<10; j++)
					{
						keycodes[i][j]=keycode_str[i*10+j];
					}
					keycodes[i][10]='\0';
				}
				
				switch(behavior)//dynamically allocate structs if needed
				{
					case hold:
					hold_data *hold_keycode;
					hold_keycode = (hold_data *) malloc (sizeof(hold_data));
					sscanf(keycodes[0], "%lx", &hold_keycode->tap);
					sscanf(keycodes[1], "%lx", &hold_keycode->hold);
					matrix[current_key].data[current_layer].keycode= (long) hold_keycode;
					matrix[current_key].data[current_layer].behavior=hold;
					break;
			
					case doubletap:
					doubletap_data *doubletap_keycode;
					doubletap_keycode = (doubletap_data *) malloc(sizeof(doubletap_data));
					sscanf(keycodes[0], "%lx", &doubletap_keycode->tap);
					sscanf(keycodes[0], "%lx", &doubletap_keycode->doubletap);
					matrix[current_key].data[current_layer].keycode= (long) doubletap_keycode;
					matrix[current_key].data[current_layer].behavior=doubletap;
					break;
			
					case dtaphold:
					dtaphold_data *dtaphold_keycode;
					dtaphold_keycode = (dtaphold_data *) malloc(sizeof(dtaphold_data));
					sscanf(keycodes[0], "%lx", &dtaphold_keycode->tap);
					sscanf(keycodes[0], "%lx", &dtaphold_keycode->doubletap);
					sscanf(keycodes[0], "%lx", &dtaphold_keycode->hold);
					matrix[current_key].data[current_layer].keycode= (long) dtaphold_keycode;
					matrix[current_key].data[current_layer].behavior=dtaphold;
					break;
				}
			}
			cout << matrix[current_key].data[current_layer].behavior << " < behavior keycode > " << 					matrix[current_key].data[current_layer].keycode << '\n';
		}
	}
	return;
}

