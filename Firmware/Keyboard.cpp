#include "Keyboard.h"

uint8_t out_buffer[8]={0};

int selector=1;

int row[NUM_ROW] = {1,21,2,3};
int col[NUM_COLL] = {4,5,6,7,8,9,10,16,14,15,18,19,20};

int custom_mods[NUM_CUSTOM_MODS] = {CUSTOM_MOD_KEY_POS};

int mapping[NUM_CUSTOM_MODS+1][NUM_KEYS] =
{
	{0x002b, 0x0014, 0x001a, 0x0008, 0x0015, 0x0017, 0x001c, 0x0018, 0x000c, 0x0012, 0x0013, 0x002a, 0x002a, 0x0029, 0x0004, 0x0016, 0x0007, 0x0009, 0x000a, 0x000b, 0x000d, 0x000e, 0x000f, 0x0033, 0x0034, 0x0028, 0x0200, 0x001d, 0x001b, 0x0006, 0x0019, 0x0005, 0x0011, 0x0010, 0x0036, 0x0037, 0x0038, 0x2000, 0x0028, 0x0100, 0x0800, 0x0400, 0x0000, 0x0000, 0x002c, 0x002c, 0x0000, 0x0000, 0x0000, 0x0000, 0x1000, 0x004c},
	{0,0x0535},
	{0x0037, 0x0024, 0x0025, 0x0026, 0x2024, 0x2025, 0x002d, 0x0031, 0x0038, 0x2026, 0x2027, 0x002a, 0x002a, 0x0027, 0x0021, 0x0022, 0x0023, 0x2021, 0x2022, 0x2023, 0x2036, 0x2037, 0x202f, 0x2030, 0x2031, 0x0028, 0x0200, 0x001e, 0x001f, 0x0020, 0x201e, 0x201f, 0x2020, 0x002e, 0x0035, 0x002f, 0x0030, 0x2000, 0x0028, 0x0100, 0x0800, 0x0400, 0x0000, 0x0000, 0x002a, 0x002a, 0x0000, 0x0000, 0x0000, 0x0000, 0x1000, 0x002c},
	{0x003a, 0x003b, 0x003c, 0x003d, 0x003e, 0x003f, 0x0040, 0x0041, 0x0042, 0x0043, 0x0044, 0x002a, 0x002a, 0x0039, 0x007f, 0x0081, 0x0080, 0x0074, 0x0078, 0x0050, 0x0051, 0x0052, 0x004f, 0x0048, 0x0045, 0x0028, 0x0200, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x004a, 0x004e, 0x004b, 0x004d, 0x0046, 0x2000, 0x0028, 0x0100, 0x0800, 0x0400, 0x0000, 0x0000, 0x0028, 0x0028, 0x0000, 0x0000, 0x0000, 0x0000, 0x1000, 0x002c},
	{0x0000, 0x0000, 0x0052, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0050, 0x0051, 0x004f, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0200, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x2000, 0x0028, 0x0100, 0x0800, 0x0400, 0x0000, 0x0000, 0x002c, 0x002c, 0x0000, 0x0000, 0x0000, 0x0000, 0x1000, 0x004c}
};



int matrix_status[2][NUM_KEYS] = {0};

int counter=0;
int toggle=0;
int toggle_map=0;
int map_use=0;
int map_use_last=0;
int modvar=0;
int modvar_pos=0;



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


// sets theh current row to HIGH
//calls thheh keycheckmethod passisng the index of the for
// resets row to write LOW
void matrix_scan()
{
  selector=!selector;

  for(int i=0; i<NUM_ROW; i++)
  {
    digitalWrite(row[i], HIGH);
    key_check(i);
    digitalWrite(row[i], LOW);
  }
  return;
}


//receives information about row being scanned
//checks each collum for a HIGH
//writes the state of each collums to the matrix_status array
//the expression simply calculates the position of the key in the array
void key_check(int i)
{

  for(int j=0; j<NUM_COLL; j++)
  {    
     if (digitalRead(col[j]) == HIGH)
    {
      matrix_status[selector][(i*NUM_COLL+j)]=1;
     }
     else matrix_status[selector][(i*NUM_COLL+j)]=0;
  }
  return;
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
     if(matrix_status[selector][i]==1) add_to_buffer(mapping[map_use][i]);    
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


//the most complicated function in the code
//it's purpose is simple however
//it checks which modifier key (if any) has been pressed
//and assign the according key map to be used this cycle
//track how long ago the last modifier key has been pressed so it can identify a double tap
//if key is dobble tapped within the specified time limit toggle that keymap until any custom modifier is pressed
//repeat
void custom_mod_handler()
{
  map_use_last=map_use;

  //this if checks whether the modifier was pressed and is being held
  if(matrix_status[selector][modvar]==1 && (counter==0))
    {
      map_use=modvar_pos+1;
    }
  else //a series of scenarios need to be adressed
  {
    //loops through all cutom mods and checks if a different mod has been pressed
    //the same mod doesn't count since it would reset the counter variable
    //in case of true then the varaibles are reset and modvar is set to the matching map
    for(int i=0; i<NUM_CUSTOM_MODS; i++)
    {
      if((matrix_status[selector][(custom_mods[i])]==1) && (custom_mods[i]!=modvar))
      {
        modvar=custom_mods[i];
        modvar_pos=i;
        counter=0;
        toggle=0;
        map_use=modvar_pos+1;
        i=NUM_CUSTOM_MODS+1;
      }
    }
  
    //this condition will be true if the user releases his last pressed modifier and hasn't pressed a new one
    //it adds to the counter that determines the double tap window
    if(matrix_status[selector][modvar]==0 && counter<100)
    {
      ++counter;
      map_use=0;
    }
    //these are the conditions nescessary for a toggle
    //the modifier has to be pressed and the counter is less than the threshold
    //the counter can't be zero otherwise this would be true as the presses the modifier
    else if(matrix_status[selector][modvar]==1 && counter<100 && counter!=0)
    {
      toggle=1;
      counter==0;
      toggle_map=modvar_pos+1;
    }
    //if counter exceeds the threshold then modvar is reset
    else if(counter>=100)
    {
      modvar=0;
    }
    //this is the default case
    else map_use=0;
  }
  if(toggle==1) map_use=toggle_map; //if toggle true uses it instead

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
