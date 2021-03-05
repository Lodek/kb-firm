/*
Decision making module for firmware. Receives current state of keys, processes the information and generates the USB report for that cycle.
Built as a module so it can be used in a wrapper program that tests its functionalities.
Wrapper/main program MUST implement "update_keys", "get_trigger" and "get_quanta" functions.

Key keys[KEYS_LEN] = {0};
Name names[NUM_LAYERS][KEYS_LEN] = NAMES_INIT;
long macros[NUM_MACROS][] = MACROS_INIT;
*/

#include "chef.h"
#include <stdlib.h>

#define REPORT_LEN 8

static int layer = 0; //int that stores which layer is being used
static int layer_value = 0; //variable with numerical value for layer
static int key_index = 0;
static int keys_len;
static int layers_len;
static long retap_delay;
static long hold_delay;
uint8_t report[REPORT_LEN] = {0};
uint8_t old_report[REPORT_LEN] = {0};
static uint8_t *hash, *old_hash;
static uint8_t *layers;
static Flags flags = {0};
static Key* keys;

void chef_init(Chef chef){
    keys_len = chef.keys_len;
    layers_len = chef.layers_len;
    retap_delay = chef.retap_delay;
    hold_delay = chef.hold_delay;
    hash = (uint8_t*) malloc(keys_len);
    old_hash = (uint8_t*) malloc(keys_len);
    layers = chef.layers;
    keys = chef.keys;
}

    

uint8_t* generate_report(){
    //Function that should be called from main. This function does all the heavy lifting
    //Returns a point to an 8 byte HID keyboard descriptor
    //Returns Null if the current and the past descriptors are equal
    for(int i = 0; i < REPORT_LEN; i++)
        old_report[i] = report[i];
    update_keys(keys);
    layer_mapper();
    update_keys_states();
    for(key_index = 0; key_index < keys_len; key_index++){
        key_handler(&keys[key_index]);
    }
    if(!report_diff())
        return 0;
    return report;
}

VerboseReport generate_verbose_report(){
    //Wrapper routine for generate_report, has aditionals returns
    //an array of pointers, first element is the addres of flags
    //second element is the layer_value
    //3rd is pointer to report
    VerboseReport rep = {flags.alt, layer_value, generate_report()};
    return rep;
}

void key_handler(Key* key){
    //Identify which Key needs attention and calls the correct handler.
    if(key->active && !key->handled){
        key_triggers trigger = get_trigger(key_index, layer);
        if(trigger == common) trigger_common(key);
        else if(trigger == hold) trigger_hold(key);
        else if(trigger == doubletap) trigger_dtap(key);
        else if(trigger == dth) trigger_dth(key);
    }
    else if(!key->active && key->remove){
        int result = report_pop(key);
        if(result) key->toggled = 0;
        else{
            key->remove = 0;
            key->handled = 0;
        }
    }
}

//Trigger handling functions
void trigger_common(Key* key){
    uint32_t quanta = get_quanta(key_index, layer, 0);
    quanta_handler(key, quanta);
}

void trigger_hold(Key* key){
    uint32_t hold_quanta = get_quanta(key_index, layer, 1);
    uint32_t tap_quanta = get_quanta(key_index, layer, 0);
    for(uint32_t i = 0; i < hold_delay; i++){
        update_keys(keys);
        update_keys_states();
        if(hash_diff() && key->active){
            //Key still held but another key has been pressed
            quanta_handler(key, hold_quanta);
            return;
        }
        else if(!key->active){
            //Key released
            quanta_handler(key, tap_quanta);
            return;
        }
    }
    //key was held for the entire time but no other actions occured
    quanta_handler(key, hold_quanta);
}

void trigger_dtap(Key* key){
    uint32_t tap_quanta = get_quanta(key_index, layer, 0);
    uint32_t dtap_quanta = get_quanta(key_index, layer, 1);
    base_triple_trigger(key, tap_quanta, dtap_quanta, tap_quanta);
    return;
}


void trigger_dth(Key* key){
    uint32_t tap_quanta = get_quanta(key_index, layer, 0);
    uint32_t dtap_quanta = get_quanta(key_index, layer, 1);
    uint32_t hold_quanta = get_quanta(key_index, layer, 2);
    base_triple_trigger(key, tap_quanta, dtap_quanta, hold_quanta);
}


void base_triple_trigger(Key* key, uint32_t tap_quanta, uint32_t dtap_quanta, uint32_t hold_quanta){
    //DRY for trigger_dtap and trigger_hold
    //their logic is identical save the quanta sent at the intervals
    uint32_t i;
    for(i = 0; i < hold_delay; i++){
        //loop waiting for a release. if key released in time
        //stuff might happen else it was held for the duration of the delay
        update_keys(keys);
        update_keys_states();
        if(hash_diff() && key->active){
            //another key has been pressed -> hold 
            quanta_handler(key, hold_quanta);
            return;
        }
        else if(!key->active) break;
    }
    if(i == hold_delay){
        //this happens if the above for runs to completions
        //indicating a hold
        quanta_handler(key, hold_quanta);
        return;
    }
    for(int j = 0; j < retap_delay; j++){
        update_keys(keys);
        update_keys_states();
        if(hash_diff() && !key->active){
            //key still released and a different key has been pressed
            quanta_handler(key, tap_quanta);
            return;
        }
        else if(key->active){
            quanta_handler(key, dtap_quanta);
            return;
        }
    }
    //Safety net. User didn't retap key nor did they tap any other key
    quanta_handler(key, tap_quanta);
}

void quanta_handler(Key* key, uint32_t quanta){
    //Handles the different types of quantas (eg toggle, normal, macro),
    //adds key to report and, if successful, updates Key's state variables.
    uint8_t type, third, second, first;
    type = quanta >> 24;
    third = quanta >> 16;
    second = quanta >> 8;
    first = quanta;
    if(type == toggle || type == normal){
        int report_index = report_append(key, third, second, first);
        if(report_index == -1) return;
        key->handled = 1;
        key->remove = 1;
        key->report_index = report_index;
        key->written_mod = second;
        key->written_layer = third;
        if(type == toggle) key->toggled = 1;
    }
    else if(type == flag){
        flag_update(first, second);
        key->handled = 1;
        return;
    }
}

int report_append(Key* key, uint8_t layer, uint8_t mod, uint8_t keycode){
    //Given the quanta, appends it to the repport.
    //Returns the index of the written keycode on the buffer
    //if report is full, returns -1 and nothing is done
    //if keycode was 0, returns 1
    //that prevents checking for abnormal values and works since keycode is 0
    int index;
    if(!keycode) index = 1;
    else{
        for(index = 2; index < 8; index++){
            if(!report[index]){
                report[index] = keycode;
                break;
            }
        }
    }
    if(index == 8) return -1;
    report[0] |= mod;
    layer_value |= layer;
    return index;
}

int report_pop(Key* key){
    //Given a key where remove is True, removes it from report and returns 0
    //if key toggled is True, returns 1 indicating that toggled should be reset
    if(key->toggled){
        //resets toggle but leaves handled as 1,
        //after being pressed and released again it will be removed
        key->toggled = 0;
        return 1;
    }
    report[key->report_index] = 0;
    report[0] &= ~key->written_mod;
    layer_value &= ~key->written_layer;
    return 0;
}

void layer_mapper(){
    //maps the value of a layer to its index in layers
    for(int i = 0; i < layers_len ; i++)
        if(layers[i] == layer_value)
            layer = i;
}

void update_keys_states(){
    //updates the hash array for the current state of keys
    uint8_t *tmp = old_hash;
    old_hash = hash;
    hash = tmp;
    for(int i = 0; i < keys_len; i++) hash[i] = keys[i].active;
}

int hash_diff(){
    //returns true if hashes are different else false
    for(int i = 0; i < keys_len; i++){
        if(old_hash[i] != hash[i]) return 1;
    }
    return 0;
}

int report_diff(){
    //returns true if current report differes from old report are different else false
    for(int i = 0; i < REPORT_LEN; i++){
        if(old_report[i] != report[i]) return 1;
    }
    return 0;
}

void flag_update(uint8_t value, uint8_t offset){
    uint8_t *flag = ((uint8_t*) &flags) + offset;
    *flag = value;
}
