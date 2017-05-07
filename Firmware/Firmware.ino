#include <HIDKeyboard.h>

#define NUM_COLL 69
#define NUM_ROW 69
#define NUM_MODS 69
#define NUM_KEYS NUM_ROW+NUM_COLL
#define NUM_CUSTOM_MODS
#define MOD_KEY_POS 1,2,3,4,5
#define CUSTOM_MOD_KEY_POS 1,2,3,4,5

void matrix_scan(); //handles matrix scan
void key_check(int, int); //checks each key for keypress
void key_sender();
void custom_mod_handler();


int selector=1;

int row[NUM_ROW] = {};
int col[NUM_COLL] = {};

int mod_keys[8] = {MOD_KEY_POS};
int custom_mods[NUM_CUSTOM_MODS] = {CUSTOM_MOD_KEY_POS};

int mapping[NUM_CUSTOM_MODS][NUM_KEYS] = {};
int matrix_status[2][NUM_KEYS] = {0};

int counter=0;
int toggle=0;
int toggle_map=0;
int map_use=0;
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
      state_list[selector][(i*NUM_COLL+j)]=1;
     }
     else state_list[selector][(i*NUM_COLL+j)]=0;
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

  write();
  return;
}

void custom_mod_handler()
{
  
  if(matrix_status[selector][modvar]==1 && counter==0)
    {
      map_use=modvar_pos+1;
    }
  else if(matrix_status[selector][modvar]==1 && toggle==1)
  {
    toggle = !toggle;
    counter = 0;
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
  
          
    if(matrix_status[selector][modvar]==0 && counter<8)
    {
      ++counter;
      map_use=0;
    }
    else if(matrix_status[selector][modvar]==1 && counter<8 && counter!=0)
    {
      toggle=1;
      counter==0;
      toggle_map=modvar_pos+1;
    }
    else mod_use=0;
  }
}

