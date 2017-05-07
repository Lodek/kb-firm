
#define NUM_COLL 3
#define NUM_ROW 2
#define NUM_KEYS NUM_ROW+NUM_COLL
#define NUM_CUSTOM_MODS 1
#define CUSTOM_MOD_KEY_POS 5
#define NUM_KEYS NUM_COLL*NUM_ROW

#include <Arduino.h>

void begin_stuff(); //starts comms with 16U2
void write_stuff(); //writes the 8 byte data structure with serial.write
void flush();
//splits the 2 bytes with info into the modifier and key bytes
uint8_t parse_mod_byte(int key); 
uint8_t parse_hidkey_byte(int key);

//receives key, parses it into the mods and key and adds or removes
// it to/from the 8 byte structure
int add_to_buffer(int key);
int remove_from_buffer(int key);


void matrix_scan(); //handles matrix scan
void key_check(int, int); //checks each key for keypress
void key_sender();
void custom_mod_handler();

uint8_t out_buffer[8]={0};

int selector=1;

int row[NUM_ROW] = {2,3};
int col[NUM_COLL] = {4,5,6};

int custom_mods[NUM_CUSTOM_MODS] = {CUSTOM_MOD_KEY_POS};

int mapping[NUM_CUSTOM_MODS+1][NUM_KEYS] = {{0x4,0x5,0x6,0x7,0x8,0},{0x9,0xA,0xB,0xC,0xD,0}};

int matrix_status[2][NUM_KEYS] = {0};

int counter=0;
int toggle=0;
int toggle_map=0;
int map_use=0;
int map_use_last=0;
int modvar=0;
int modvar_pos=0;

void setup() 
{
  // keyboard.begin();
  
  for(int i=0; i<NUM_ROW; i++) //sets row pins as OUT
  {
    pinMode(row[i], OUTPUT);
    digitalWrite(row[i], LOW);
  }

  for(int i=0; i<NUM_COLL; i++) //sets colls pins as IN
  {
    pinMode(col[i], INPUT);
  }
  begin_stuff();
    
}

void loop() 
{
  selector=!selector;
  matrixScan();
  custom_mod_handler();
  key_sender();
}

void keycheck(int rowPinN, int i)
{
  digitalWrite(rowPinN, HIGH);
  
  for(int j=0; j<NUM_COLL; j++)
  {    
     if (digitalRead(col[j]) == HIGH)
    {
      matrix_status[selector][(i*NUM_COLL+j)]=1;
     }
     else matrix_status[selector][(i*NUM_COLL+j)]=0;
  }
   

  digitalWrite(rowPinN, LOW);
  return;
}

void matrixScan()
{
  for(int i=0; i<NUM_ROW; i++)
  {
    keycheck(row[i], i);
  }
  return;
}

void key_sender()
{
  //uses modvar
  //loops through matrix scan and sends it to add buffer using correct map
  
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
  write_stuff();
  return;
}

void custom_mod_handler()
{
  map_use_last=map_use;

  if(matrix_status[selector][modvar]==1 && (counter==0))
    {
      map_use=modvar_pos+1;
    }
  else
  {
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
  
          
    if(matrix_status[selector][modvar]==0 && counter<100)
    {
      ++counter;
      map_use=0;
    }
    else if(matrix_status[selector][modvar]==1 && counter<100 && counter!=0)
    {
      toggle=1;
      counter==0;
      toggle_map=modvar_pos+1;
    }
    else if(counter>=100)
    {
      modvar=0;
    }
    else map_use=0;
  }
  if(toggle==1) map_use=toggle_map;
}



//begins commns
void begin_stuff()
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

void write_stuff()
{
  HID_SendReport(2, out_buffer, 8);
}  

void flush()
{
  for(int i=0; i<8; i++) out_buffer[i]=0;
  return;
}

