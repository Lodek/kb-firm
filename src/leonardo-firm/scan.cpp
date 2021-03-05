#include "scan.h"
#include "maps.h"
#include "defines.h"
#include <Arduino.h>
#include <avr/pgmspace.h>

#define HAS_ANALOG 0

Name names[LAYERS_LEN][KEYS_LEN] = NAMES;
DIMS
uint32_t* quantas[QUANTAS_LEN] = QUANTAS;

uint8_t inputs[INPUTS_LEN] = {INPUT_PINS};
uint8_t outputs[OUTPUTS_LEN] = {OUTPUT_PINS};

void init_gpio(){
    for (int i = 0; i < OUTPUTS_LEN; i++){
        pinMode(outputs[i], OUTPUT);
        digitalWrite(outputs[i], OUTPUT_OFF);
    }
    for (int i = 0; i < INPUTS_LEN; i++){
        pinMode(inputs[i], INPUT);
        if(INPUT_PULLUP)
            digitalWrite(inputs[i], HIGH);
    }
}

void update_keys(Key* keys){
    for(int i = 0; i < OUTPUTS_LEN; i++){
        digitalWrite(outputs[i], OUTPUT_ON);
        for(int j = 0; j < INPUTS_LEN; j++){
            int index = i * INPUTS_LEN + j;
            if(digitalRead(inputs[j]) == OUTPUT_ON)
                keys[index].active = 1;
            else
                keys[index].active = 0;
        }
        digitalWrite(outputs[i], OUTPUT_OFF);
    }
}
                     
Name* get_name(int index, int layer){
    return 1;
}

uint8_t get_trigger(int index, int layer){
    return names[layer][index].trigger;
}

uint32_t get_quanta(int index, int layer, int offset){
    int quanta_index = names[layer][index].quanta_index + offset;
    return pgm_read_dword(quantas[layer] + quanta_index);
}
