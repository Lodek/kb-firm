#include "scan.h"
#include "maps.h"
#include "defines.h"
#include <stdio.h>

Name names[LAYERS_LEN][KEYS_LEN] = NAMES;
DIMS
uint32_t* quantas[QUANTAS_LEN] = QUANTAS;

extern int input_eof;
extern FILE* file;

void update_keys(Key* keys){
    if(input_eof)
        return;
    for(int i = 0, c = 0; c != '\n'; i++){
        c = getc(file);
        if(c == EOF){
            fclose(file);
            input_eof = 1;
            return;
        }
        if(c == '0')
            keys[i].active = 0;
        else
            keys[i].active = 1;
    }
}


Name* get_name(int index, int layer){
    Name name;
    return &names[layer][index];
}

uint8_t get_trigger(int index, int layer){
    return names[layer][index].trigger;
}

uint32_t get_quanta(int index, int layer, int offset){
    int quanta_index = names[layer][index].quanta_index;
    uint32_t* dimension = quantas[layer];
    return dimension[quanta_index+offset];
}
