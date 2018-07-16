/*
Decision making module for firmware. Receives current state of keys, processes the information and generates the USB report for that cycle.
Built as a module so it can be used in a wrapper program that tests its functionalities.
Wrapper program MUST implement an "update_keys" and "get_data" functions.
*/
#include "keyboard.h"
#include "mappings.h"

const uint8_t layers[LAYERS_LEN] = {LAYER_VALUE};
uint8_t layer = 0, layer_value = 0; //int that stores which layer is being used

uint8_t report[8] = {0};

char hash[KEYS_LEN] = 0;
char old_hash[KEYS_LEN] = 0;

uint8_t* generate_report(Key** keys){
    layer_mapper();
    gen_hash();
    for(int index = 0; index < KEYS_LEN; index++){
        key_handler(&keys[index]);
    }
    return report;
}

void key_handler(Key* key){
    //Identify which keys need attention (ie removal and insertion)
    //and calls the correct handler.
    if(key->active && !key->handled){
        uint8_t trigger = key->names[layer].trigger;
        if(trigger == normal) trigger_normal(key);
        else if(trigger == hold) trigger_hold(key);
        else if(trigger == doubletap) trigger_doubletap(key);
        else if(trigger == dth) trigger_dth(key);
    }
    else if(!key->status && key->remove){
        uint8_t result = pop_report(key);
        if(result) key->toggled = 0;
        else{
            key->remove = 0;
            key->handled = 0;
        }
    }
    return;
}

//Trigger handling functions
void trigger_normal(Key* key){
    long data = get_data(get_index(key), 0);
    secrets(key, data);
}

void trigger_hold(Key* key){
    long hold_data = get_data(get_index(key), 0);
    long tap_data = get_data(get_index(key), 1);
    for(long i = 0; i < HOLD_DELAY; i++){
        update_keys();
        gen_hash();
        if(hash_diff() && key->status){
            //Key still held but another key has been pressed
            secrets(key, hold_data);
            return;
        }
        else if(!key->status){
            //Key released
            secrets(key, tap_data);
            return;
        }
    }
    //key was held for the entire time but no other actions occured
    secrets(key, hold_data);
    return;
}

void trigger_dtap(Key* key){
    long tap_data = get_data(get_index(key), 0);
    long dtap_data = get_data(get_index(key), 1);
    base_tripple_trigger(key, tap_data, dtap_data, tap_data);
    return;
}


void trigger_dth(Key* key){
    long tap_data = get_data(get_index(key), 0);
    long dtap_data = get_data(get_index(key), 1);
    long hold_data = get_data(get_index(key), 2);
    base_tripple_trigger(key, tap_data, dtap_data, hold_data);
    return;
}


void base_triple_trigger(Key* key, long tap_data, long dtap_data, long hold_data){
    //DRY for trigger_dtap and trigger_hold
    //their logic is identical save the data sent at the intervals
    for(long i = 0; i < HOLD_DELAY; i++){
        //loop waiting for a release. if key released in time
        //stuff might happen else it was held for the duration of the delay
        update_keys();
        gen_hash();
        if(hash_diff() && key->active){
            //another key has been pressed -> hold 
            secrets(key, hold_data);
            return;
        }
        else if(!key->active) break;
    }
    if(i == HOLD_DELAY){
        //this happens if the above for runs to completions
        //indicating a hold
        secrets(key, hold_data);
        return;
    }
    for(int j = 0; i <RETAP_DELAY; i++){
        update_keys();
        if(hash_diff() && !key->active){
            //key still released and a different key has been pressed
            secrets(key, tap_data);
            return;
        }
        else if(key->active){
            secrets(key, dtap_data);
            return;
        }
    }
    secrets(key, tap_data);
    return;
}

void secrets(Key* key, long data){
    uint8_t type = data >> 32;
    if(type == toggle || type == normal){
        uint8_t report_index = append_report(key, payload);
        if(buffer_index == -1) return;
        key->handled = 1;
        key->remove = 1;
        key->report_index = report_index;
        key->written_mod = data >> 8;
        key->written_layer = data >> 16;
        if(type == toggle) key->toggled = 1;
    }
    //else cry
    return;
}

int append_report(Key* key, long data){
    //Given the data, appends it to the repport.
    //Returns the index of the written keycode on the buffer
    //if report is full, returns -1 and nothing is done
    //if keycode was 0, returns 1
    //that prevents checking for abnormal values and works since keycode is 0
    uint8_t layer, mod, keycode;
    layer = data >> 16;
    mod = data >> 8;
    keycode = data;
    int index;
    if(!keycode) index = 1;
    else{
        for(index = 2; i < 8; i++){
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

uint8_t pop_report(Key* key){
    //Given a key where remove is True, erases its data from report and returns 0
    //if key toggled is True, returns 1 indicating that toggled should be reset
    if(key->toggled){
        //resets toggle but leaves handled intact,
        //after being pressed and released again it will be removed
        key->toggled = 0;
        return;
    }
    report[index] = key->report_index;
    report[0] &= ~key->written_mod;
    layer_value &= ~key->written_layer;
    return;
}

uint8_t get_index(Key* key){
    //Returns the value of index of key for the active layer
    return key->names[layer].index
}

void layer_mapper(){
    //maps the value of a layer to its index in layers
    for (uint8_t i = 0; i < NUM_LAYERS ; i++) if (layers[i] == layer_value) layer=i;
    return;
}

void gen_hash(Key** keys){
    //updates the hash array for the current state of keys
    char* temp = old_hash;
    old_hash = hash;
    hash = old_hash;
    for(int i = 0; i < KEYS_LENGTH; i++) hash[i] = keys[i]->active;
    return;
}

uint8_t hash_diff(){
    //returns true if hashes are different else false
    for(int i=0; i<1+NUM_KEYS/8; i++){
        if(old_hash[i] != hash[i]) return 1;
    }
    return 0;
}
