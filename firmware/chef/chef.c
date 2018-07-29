/*
Decision making module for firmware. Receives current state of keys, processes the information and generates the USB report for that cycle.
Built as a module so it can be used in a wrapper program that tests its functionalities.
Wrapper/main program MUST implement "update_keys", "get_trigger" and "get_quanta" functions.

Key keys[KEYS_LEN] = {0};
Name names[NUM_LAYERS][KEYS_LEN] = NAMES_INIT;
long macros[NUM_MACROS][] = MACROS_INIT;
*/

#include "chef.h"
#include "scan.h"
#include "defines.h"
#include "maps.h"

_byteint layer = 0; //int that stores which layer is being used
_byteint layer_value = 0; //variable with numerical value for layer
_byteint report[8] = {0};
_byteint h1[100] = {0};
_byteint h2[100] = {0};
_byteint key_index;
_byteint *hash = h1;
_byteint *old_hash = h2;
_byteint layers[LAYERS_LEN] = {LAYERS_VAL};


_byteint* generate_report(){
    layer_mapper();
    gen_hash();
    for(key_index = 0; key_index < KEYS_LEN; key_index++){
        key_handler(&keys[key_index]);
    }
    return report;
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
        _byteint result = report_pop(key);
        if(result) key->toggled = 0;
        else{
            key->remove = 0;
            key->handled = 0;
        }
    }
}

//Trigger handling functions
void trigger_common(Key* key){
    long quanta = get_quanta(key_index, layer, 0);
    quanta_handler(key, quanta);
}

void trigger_hold(Key* key){
    long hold_quanta = get_quanta(key_index, layer, 0);
    long tap_quanta = get_quanta(key_index, layer, 1);
    for(long i = 0; i < HOLD_DELAY; i++){
        update_keys();
        gen_hash();
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
    long tap_quanta = get_quanta(key_index, layer, 0);
    long dtap_quanta = get_quanta(key_index, layer, 1);
    base_triple_trigger(key, tap_quanta, dtap_quanta, tap_quanta);
    return;
}


void trigger_dth(Key* key){
    long tap_quanta = get_quanta(key_index, layer, 0);
    long dtap_quanta = get_quanta(key_index, layer, 1);
    long hold_quanta = get_quanta(key_index, layer, 2);
    base_triple_trigger(key, tap_quanta, dtap_quanta, hold_quanta);
}


void base_triple_trigger(Key* key, long tap_quanta, long dtap_quanta, long hold_quanta){
    //DRY for trigger_dtap and trigger_hold
    //their logic is identical save the quanta sent at the intervals
    long i;
    for(i = 0; i < HOLD_DELAY; i++){
        //loop waiting for a release. if key released in time
        //stuff might happen else it was held for the duration of the delay
        update_keys();
        gen_hash();
        if(hash_diff() && key->active){
            //another key has been pressed -> hold 
            quanta_handler(key, hold_quanta);
            return;
        }
        else if(!key->active) break;
    }
    if(i == HOLD_DELAY){
        //this happens if the above for runs to completions
        //indicating a hold
        quanta_handler(key, hold_quanta);
        return;
    }
    for(int j = 0; j < RETAP_DELAY; j++){
        update_keys();
        gen_hash();
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

void quanta_handler(Key* key, long quanta){
    //Handles the different types of quantas (eg toggle, normal, macro),
    //adds key to report and, if successful, updates Key's state variables.
    _byteint type, layer, mod, keycode;
    type = quanta >> 32;
    layer = quanta >> 16;
    mod = quanta >> 8;
    keycode = quanta;
    if(type == toggle || type == normal){
        _byteint report_index = report_append(key, layer, mod, keycode);
        if(report_index == -1) return;
        key->handled = 1;
        key->remove = 1;
        key->report_index = report_index;
        key->written_mod = mod;
        key->written_layer = layer;
        if(type == toggle) key->toggled = 1;
    }
    else{
        int cries_out_loud_because_I_havent_done_this_yet = 1;
    }
}

_byteint report_append(Key* key, _byteint layer, _byteint mod, _byteint keycode){
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

_byteint report_pop(Key* key){
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
    for(_byteint i = 0; i < LAYERS_LEN ; i++) if (layers[i] == layer_value) layer=i;
}

void gen_hash(){
    //updates the hash array for the current state of keys
    _byteint *tmp = old_hash;
    old_hash = hash;
    hash = tmp;
    for(int i = 0; i < KEYS_LEN; i++) hash[i] = keys[i].active;
}

_byteint hash_diff(){
    //returns true if hashes are different else false
    for(int i=0; i < KEYS_LEN; i++){
        if(old_hash[i] != hash[i]) return 1;
    }
    return 0;
}
