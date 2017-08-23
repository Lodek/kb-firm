#include "Keyboard.h"

key matrix[NUM_KEYS]; //data structure with an array of key objects
//essentially the whole code

int layervar = 0; //int that stores which layer is being used

uint8_t out_buffer[8] = {0}; //buffer to output USB data

//physical pin number for each row and collunm
int row_pins[NUM_ROW] = {1, 21, 2, 3};
int col_pins[NUM_COLL] = {4, 5, 6, 7, 8, 9, 10, 16, 20, 15, 19, 14, 18};

//array that makes the matching of index number of layer with the value utilized by layer var
int layers[NUMLAYERS] = {LAYERVALUE};

//user defined mappings for each key
long mapping[NUMLAYERS][NUM_KEYS] =
{
	{0x002b, 0x0014, 0x001a, 0x0008, 0x0015, 0x0017, 0x001c, 0x0018, 0x000c, 0x0012, 0x0013, 0x002a, 0x002a, 0x0029, 0x0004, 0x0016, 0x0007, 0x0009, 0x000a, 0x000b, 0x000d, 0x000e, 0x000f, 0x0033, 0x0034, 0x0028, 0x0200, 0x001d, 0x001b, 0x0006, 0x0019, 0x0005, 0x0011, 0x0010, 0x0036, 0x0037, 0x0038, 0x2000, 0x0028, 0x0100, 0x0800, 0x0400, 0x0000, 0x010000, 0x002c, 0x002c, 0x020000, 0x0000, 0x0000, 0x0000, 0x1000, 0x004c},
	{0x0037, 0x0024, 0x0025, 0x0026, 0x2024, 0x2025, 0x002d, 0x0031, 0x0038, 0x2026, 0x2027, 0x002a, 0x002a, 0x0027, 0x0021, 0x0022, 0x0023, 0x2021, 0x2022, 0x2023, 0x2036, 0x2037, 0x202f, 0x2030, 0x2031, 0x0028, 0x0200, 0x001e, 0x001f, 0x0020, 0x201e, 0x201f, 0x2020, 0x002e, 0x0035, 0x002f, 0x0030, 0x2000, 0x0028, 0x0100, 0x0800, 0x0400, 0x0000,0x010000, 0x002a, 0x002a, 0x020000, 0x0000, 0x0000, 0x0000, 0x1000, 0x002c}
};



// initialize microcontroller
// sets rows as outputs and colls as inputs
// begins serial communication
void startup_routine()
{

  for (int i = 0; i < NUM_ROW; i++) //sets row pins as OUT
  {
    pinMode(row_pins[i], OUTPUT);
    digitalWrite(row_pins[i], LOW);
  }

  for (int i = 0; i < NUM_COLL; i++) //sets colls pins as IN
  {
    pinMode(col_pins[i], INPUT);
  }
  _begin();
  init_mappings();
  return;
}



//uses col and row array with corresponding pin number
//loops through each row by setting it to high, calls digital  read on each collumn
//write result of digital read to the matching key on the matrix array
//resets current row to low
//repeats for next row
void matrix_scan()
{

  for (int i = 0; i < NUM_ROW; i++)
  {
    digitalWrite(row_pins[i], HIGH);
    for (int j = 0; j < NUM_COLL; j++)
    {
      matrix[i * NUM_COLL + j].state = digitalRead(col_pins[j]);
    }
    digitalWrite(row_pins[i], LOW);
  }
  return;
}





//calls the key handler for each element of the matrix array
void matrix_iterator()
{
  for (int i = 0; i < NUM_COLL * NUM_ROW; i++)
  {
    key_handler(&matrix[i]);
  }
  return;
}

//key handler receives a key struct object
//it first checks whether the key needs to be handled
//then it checks the key behavior and sends calls the appropriate handler function for that key type
void key_handler(key *current_key)
{
  if (current_key->state == 0 && current_key->important == 0) return;

  if (current_key->data[layervar_translator(layervar)].behavior == normal) behavior_normal(current_key);

  return ;
}

void behavior_normal (key *current_key)
{
  long keycode;

  if (current_key->state == 1 && current_key->important == 1) return; //key was and still is pressed, nothig to be done

  if (current_key->state == 0 && current_key->important == 1) //is called when key has been released but its info is still on buffer and/or layervar
  {
    keycode = current_key->on_buffer;

    remove_from_buffer(keycode);

    current_key->on_buffer = -1;
    current_key->important = 0;
  }

  else if (current_key->state == 1 && current_key->important == 0) //key pressed and not on buffer
  {
    keycode = current_key->data[layervar_translator(layervar)].keycode;

    current_key->on_buffer = add_to_buffer(keycode); //calls add to buffer and writes the return to on_buffer variable

    if (current_key->on_buffer != -1)
      //add to buffer returns -1 if buffer is full. if buffer is full important remains 0 and key is proccessed next cycle
    {
      //sets the 2nd byte of on buffer with HID Modifiers used and the 3rd byte with layer value. Info needed for effective removal from buffer
      current_key->on_buffer = ((keycode & 0xFFFF00) | current_key->on_buffer);

      current_key->important = 1; //important=1 is important....
    }
  }
  return;
}



//writes a key to the first empty spot found on the buffer
//returns k if added, returns -1 if the buffer was full
//ors modifiers to append them
//ors layers with layervar to append layer
int add_to_buffer(long key)
{

  int result = -1;

  //search for empty spot on output buffer and writes to it
  for (int k = 2; k < 8; k++)
  {
    if (out_buffer[k] == 0)
    {
      out_buffer[k] = (key&0x0000FF);
      result = k;
      k = 9;

    }
  }

  layervar = (key >> 16) | layervar; //ORs layer byte value of keycode to layervar

  //OR's modifiers byte to add
  //bit shifts 8 to the right (position it on the first byte) AND with 0x0000FF to get rid of layer byte
  //finaly OR with out_buffer to append new modifier

  out_buffer[0] = out_buffer[0] | ((key >> 8) & 0x0000FF);
  return result;
}



//removes the received key from the buffer
int remove_from_buffer(long key)
{
  uint8_t mods, bufferpos;


  bufferpos = (key & 0x0000FF); //isolates first byte of key to get buffer index of the given key
  mods = ((key >> 8) & 0x0000FF); //isolates second byte of key to get modifiers written by that key
  layervar = (key >> 16)^layervar; //isolates the layer byte from keycode and XOR with layer var to remove it

  out_buffer[bufferpos] = 0; //erases out buffer using isolated buffer pos

  //XOR's modifiers byte to subtract new ones
  out_buffer[0] = out_buffer[0] ^ mods;
  return 0;
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
  for (int i = 0; i < 8; i++) out_buffer[i] = 0;
  return;
}

//Utility functions



//begins commns
//no longer needed for pro micro
//still used for uno and mega
void _begin()
{
  Serial.end();
  Serial.begin(9600);
}


//function that the index of the corresponding VALUE of a layer (1,2,4,8...)
int layervar_translator(int layer)
{
  for (int i = 0; i < NUMLAYERS ; i++)
  {
    if (layers[i] == layer) return i;
  }
}

void init_mappings()
{
  for (int i = 0; i < NUM_KEYS; i++)
  {
    matrix[i].important = 0;
    matrix[i].on_buffer = 0;
    matrix[i].state = 0;
    for (int j = 0; j < NUMLAYERS; j++)
    {
      matrix[i].data[j].behavior = normal;
      matrix[i].data[j].keycode = mapping[j][i];
    }
  }
  return;
}
