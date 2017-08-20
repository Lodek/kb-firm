#include "Keyboard.h"

#define NUMLAYERS
#define LAYERVALUE
typedef enum behavior_enum {normal, hold, doubletap, clear, mod, macro, dth, ghost} behavior_enum; 

typedef struct key_data
{
	behavior_enum behavior;
	int keycode;
} key_data;

typedef struct key
{
	int important, on_buffer, state;
	
	key_data data[NUMLAYERS+1];
	
} key;

key matrix[NUMKEYS];

int modstate;

uint8_t out_buffer[8]={0};

int row[NUM_ROW] = {1,21,2,3};
int col[NUM_COLL] = {4,5,6,7,8,9,10,16,14,15,18,19,20};


int mapping[NUM_CUSTOM_MODS+1][NUM_KEYS] =
{
};


// initialize microcontroller
// sets rows as outputs and colls as inputs
// begins serial communication
void startup_routine()
{
  
  for(int i=0; i<NUM_ROW; i++) //sets row pins as OUT
  {
    pinMode(row[i], OUTPUT);
    digitalWrite(row[i], LOW);
  }

  for(int i=0; i<NUM_COLL; i++) //sets colls pins as IN
  {
    pinMode(col[i], INPUT);
  }
  _begin();
  return;
}

//uses col and row array with corresponding pin number
//loops through each row by setting it to high, calls digital  read on each collumn
//write result of digital read to the matching key on the matrix array
//resets current row to low
//repeats for next row
void matrix_scan()
{

  for(int i=0; i<NUM_ROW; i++)
  {
    digitalWrite(row[i], HIGH);
    for(int j=0; j<NUM_COL; j++)
	{
		matrix[i*NUM_COLL+j].state=digitalRead(col[j]);
	}
    digitalWrite(row[i], LOW);
  }
  return;
}

void layer_selector()
{

}



//checks if the current keymap is different than the last used
//if different flushes the buffer and write the current list of HIGHs to the buffer
//else loops through matrix_status
//checks if key was pressed and is no longer pressed to remove it from buffer
//else if key wasn't pressed and is now pressed to add to buffer
//finally it writes the buffer
void key_sender()
{
  
  if(map_use!=map_use_last) 
  {
   flush();
   for(int i=0; i<NUM_COLL*NUM_ROW; i++)
   {
     if(matrix_status[selector][i]==1)	
	 {
		if(profile[map_use][i].key_type==0) add_to_buffer(profile[0][i].key_code);
	 }
   }
  }
   
   else
   {

     for(int i=0; i<NUM_COLL*NUM_ROW; i++)
       {
         if((matrix_status[selector][i]==1) && (matrix_status[!selector][i]==0))
           {
             add_to_buffer(mapping[map_use][i]);
           }

         else if((matrix_status[selector][i]==0) && (matrix_status[!selector][i]==1))
           {
             remove_from_buffer(mapping[map_use][i]);
           }
       }
  }
  write_buffer();
  return;
}






//USB related functions










//begins commns
//no longer needed for pro micro
//still used for uno and mega
void _begin()
{
  Serial.end(); 
  Serial.begin(9600);
}



//isolates the second byte from the int
//the result is the modifier byte

uint8_t parse_mod_byte(int key)
{
  //bit shifts the int 8bits to the right to get rid of HID key
  //ands the first byte to get mods
  return ((key>>8)&0x00FF); 
}  



//isolates the first byte from the int
//the result is the hex of the HID standard key code

uint8_t parse_hidkey_byte(int key)
{
  //ANDs the first byte with TRUE to keep the hid key
  //ANDs the second byte with FALSE to throw away mods
  return ((key)&0x00FF);
}  



//writes a key to the first empty spot found on the buffer
//ors modifiers to append them
//returns 0 if added returns 1 if the buffer was full

int add_to_buffer(int key)
{
  uint8_t mods,hidkey;
  int result=1;

  hidkey=parse_hidkey_byte(key);
  mods=parse_mod_byte(key);

  //search for empty spot on output buffer and writes to it
  for(int k=2;k<8;k++)
  { 
    if(out_buffer[k]==0) 
    {
       out_buffer[k]=hidkey;
       k=9;
       result=0;
    }
  }
  //OR's modifiers byte to add em up
  out_buffer[0]=out_buffer[0]|mods;
  return result;
}



//removes the received key from the buffer
//parses key and search for it on buffer
//if found it is deleted
//XOR's mods to subtract them
//returns 1 if key was not found on buffer
int remove_from_buffer(int key)
{
  uint8_t mods,hidkey;
  int result=1;

  hidkey=parse_hidkey_byte(key);
  mods=parse_mod_byte(key);

//searchs for a keymatch on the buffer then removes it
  for(int k=2;k<8;k++)
  { 
    if(out_buffer[k]==hidkey) 
    {
      out_buffer[k]=0;
      k=9;
      result=0;
    }
  }
//XOR's modifiers byte to subtract new ones
  out_buffer[0] = out_buffer[0]^mods;
  return result;
}


//Sends the USB report to the host, the data element of the report is out_buffer which has length 8

void write_buffer()
{
  HID_SendReport(2, out_buffer, 8);
  return;
}  


//loops through out_buffer zeroing all elements of the array
void flush()
{
  for(int i=0; i<8; i++) out_buffer[i]=0;
  return;
}

