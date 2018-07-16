#include "defines.h"

//Defines the meaning of the 4th byte of the keycode
//the key will be handled according to its type
typedef enum {normal, toggle, macro, dead, absolute} keycode_type;

//Possible triggers for a physical key switch.
// this data is used to proccess a key presses
typedef enum {normal, hold, doubletap, dth} key_triggers;

typedef struct {
    //active -> indicate whether key is pressed or not
    //handled -> key has been dealt with
    //remove -> marked for removal
    //toggled -> aux bit to handle toggles
    uint8_t active, handled, remove, toggled;
    uint8_t written_mod, written_layer, report_index;
    //Keys are complicated beings, they have various names
    //one name for each layer. Each name contains bits of its existence
    //such as what triggers that key on that layer and the payload
	key_name names[NUM_LAYERS];
} Key;

typedef struct {
    //Keys have as many names as there are layers, we can only call one name at a time
    //their name tells us its trigger and their homes
    key_triggers trigger; //activation modes for key
    uint8_t index; //index of the key in the array with keycodes
} key_name;

uint8_t* generate_report(Key** keys);
void key_handler(Key* key);
void trigger_normal(Key* key);
void trigger_hold(Key* key);
void trigger_dtap(Key* key);
void trigger_dth(Key* key);
void base_triple_trigger(Key* key, long tap_data, long dtap_data, long hold_data);
void secrets(Key* key, long data);
int append_report(Key* key, long data);
uint8_t pop_report(Key* key);
uint8_t get_index(Key* key);
void layer_mapper();
void gen_hash(Key** keys);
uint8_t hash_diff();
