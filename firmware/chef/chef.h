#include "defines.h"

//Defines the meaning for quanta's 4th byte
typedef enum {normal, toggle, macro, dead, absolute} quanta_type;

//Possible triggers for a physical key switch.
typedef enum {normal, hold, doubletap, dth} key_triggers;

typedef struct {
    //active -> indicate whether key is pressed or not
    //handled -> key has been dealt with
    //remove -> marked for removal
    //toggled -> aux bit to handle toggles
    uint8_t active, handled, remove, toggled;
    uint8_t written_mod, written_layer, report_index;
    uint8_t index, trigger;
    //Conceptually, Key should have a Name for each layer.
    //Due to hardware limitations however (ie runtime SRAM), Names were moved to ROM,
    //thus it was separated from Key since Key contains state variables.
    //It is helpful to think of Key as containing Names.
} Key;

typedef struct {
    //Keys are complicated beings, they have various names, each of which contains bits of its existence.
    //Each name defines what that key does in that layer.
    //Name contains that key's Quantas and triggers for that layer.
    const key_triggers trigger;
    const long quantas[];
} Name;

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
