#define NUM_COLL 13
#define NUM_ROW 4
#define NUM_PINS NUM_ROW+NUM_COLL
#define NUM_KEYS NUM_COLL*NUM_ROW
#define NUMLAYERS 1
#define LAYERVALUE 0
#include <Arduino.h>

//possible behaviors of a key
typedef enum
{
normal, hold, doubletap, clear, macro, dth, dead
} behavior_enum; 

//struct that stores the information of each key for each layer
typedef struct
{
  //defines the behavior of said key
  behavior_enum behavior;
    int keycode; 
  //contains the 3 byte code that carries the HID scan code, HID modifier code and layer value
  //it can also contain the pointer to an struct for different key behaviors
} key_data;

//actual definition of a key
//contains state variables and an array of type key_data
//the array carries the data/behavior of the key for each layer defined by the user
typedef struct
{
  int important, on_buffer, state;
  
  key_data data[NUMLAYERS];
  
} key;

void startup_routine(); //sets up the pins in the arduino
void _begin(); //serial?
void init_mappings();
int layervar_translator(int layer);

void matrix_scan();
void matrix_iterator();
void key_handler(key current_key);

void behavior_normal(key current_key);

int add_to_buffer(int key);
int remove_from_buffer(int key);
void flush();//zero out buffer
void write_buffer();//sends buffer to host


