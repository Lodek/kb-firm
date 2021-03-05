#ifndef CHEF_H
#define CHEF_H

#include <stdint.h>

//Defines the meaning for quanta's 4th byte
typedef enum {normal, toggle, macro, dead, absolute, flag, signal} quanta_type;

//Possible triggers for a physical key switch.
typedef enum {common, hold, doubletap, dth} key_triggers;



typedef struct {
    //active -> indicate whether key is pressed or not
    //handled -> key has been dealt with
    //remove -> marked for removal
    //toggled -> aux bit to handle toggles
    uint8_t active, handled, remove, toggled;

    //counts the number of cycles the key has been active for
    uint8_t active_cycles_count;

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
    //Name Contains That key's Quantas and triggers for that layer.
    //Again, due to hardware limitations, I had to separate quantas from names
    //and put it into its own array to be stored in progmem.
    key_triggers trigger;
    uint8_t quanta_index;
} Name;

typedef struct{
    uint8_t alt;
} Flags;

typedef struct{
    uint8_t alt;
    uint8_t layer_values;
    uint8_t* report;
} VerboseReport;
    
typedef struct{
    uint8_t keys_len, layers_len;
    uint8_t* layers;
    Key* keys;
    long retap_delay, hold_delay;
} Chef;

void chef_init(Chef chef);
uint8_t* generate_report();
VerboseReport generate_verbose_report();
void key_handler(Key* key);
void trigger_common(Key* key);
void trigger_hold(Key* key);
void trigger_dtap(Key* key);
void trigger_dth(Key* key);
void base_triple_trigger(Key* key, uint32_t tap_quanta, uint32_t dtap_quanta, uint32_t hold_quanta);
void quanta_handler(Key* key, uint32_t quanta);
int report_append(Key* key, uint8_t layer, uint8_t mod, uint8_t keycode);
int report_pop(Key* key);
void layer_mapper();
void gen_hash();
int hash_diff();
int report_diff();
uint8_t alt_report();
void flag_update(uint8_t value, uint8_t offset);

uint8_t get_trigger(int index, int layer);
uint32_t get_quanta(int index, int layer, int offset);
void update_keys(Key* keys);

#endif
