#include "usb_handler.h"

uint8_t out_buffer[8]={0};

//begins commns
void _begin()
{
  Serial.end(); 
  Serial.begin(9600);
}

//isolates the second byte from the int
//the result is the modifier byte
uint8_t parse_mod_byte(int key)
{
  return ((key>>8)&0xFFFF);
}  

//isolates the first byte from the int
//the result is the hex of the HID standard key code
uint8_t parse_hidkey_byte(int key)
{
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
  out_buffer[1]=out_buffer[1]|mod;
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
  out_buffer[1] = out_buffer[1]^mods;
  return result;
}

void write_buffer()
{
  HID_SendReport(2, out_buffer, 8);
}  

void flush()
{
  for(int i=0; i<8; i++) out_buffer[i]=0;
  return;
}

