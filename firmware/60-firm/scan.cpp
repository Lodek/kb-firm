#include "scan.h"
#include "maps.h"
#include "defines.h"
#include <Arduino.h>

const Name names[LAYERS_LEN][KEYS_LEN] PROGMEM = NAMES;
DIMS
uint32_t* quantas[QUANTAS_LEN] = QUANTAS;

uint8_t inputs[INPUTS_LEN] = {INPUT_PINS};
uint8_t outputs[OUTPUTS_LEN] = {OUTPUT_PINS};

void init_gpio(){
    for (int i = 0; i < OUTPUTS_LEN; i++){
        pinMode(outputs[i], OUTPUT);
        digitalWrite(outputs[i], LOW);
    }
    for (int i = 0; i < INPUTS_LEN; i++)
        pinMode(inputs[i], INPUT);
}

void update_keys(Key* keys){
    for(int i = 0; i < OUTPUTS_LEN; i++){
        digitalWrite(outputs[i], HIGH);
        for(int j = 0; j < INPUTS_LEN; j++){
            int index = i * INPUTS_LEN + j;
            keys[index].active = digitalRead(inputs[j]);
        }
        digitalWrite(outputs[i], LOW);
    }
}


Name* get_name(int index, int layer){
    Name name;
    memcpy_P(&name, &names[layer][index], sizeof(Name));
    return &name;
}

uint8_t get_trigger(int index, int layer){
    Name name;
    memcpy_P(&name, &names[layer][index], sizeof(Name));
    return name.trigger;
}

uint32_t get_quanta(int index, int layer, int offset){
    Name name;
    memcpy_P(&name, &names[layer][index], sizeof(Name));
    return pgm_read_dword_near(quantas[layer][name.quanta_index] + 4 * offset);
}
