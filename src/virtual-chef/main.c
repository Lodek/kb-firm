#include <stdio.h>
#include <stdint.h>
#include "chef.h"
#include "maps.h"
#include "defines.h"


void print_verbose_report(VerboseReport rep);
void print_report(uint8_t* rep);

int input_eof = 0;
FILE* file;
Key keys[KEYS_LEN] = {0};

int main(int argc, char** argv){
    uint8_t layers[LAYERS_LEN] = {LAYERS_VAL};
    Chef chef = {KEYS_LEN, LAYERS_LEN, layers, keys, RETAP_DELAY, HOLD_DELAY};
    chef_init(chef);
    if(argc > 1)
        file = fopen(argv[1], "r");
    else
        file = fopen("input", "r");
    while(!input_eof){
        VerboseReport rep = generate_verbose_report();
        print_verbose_report(rep);
    }
    return 0;
}


void print_report(uint8_t* rep){
    printf("[");
    if(rep == 0){
        printf("NULL]  ");
    }
    else{
        for(int i = 0; i < 8-1; i++)
            printf("%d,", rep[i]);
        printf("%d]  ", rep[7]);
    }
    for(int i = 0; i < KEYS_LEN; i++)
        printf("%d", keys[i].active);
    printf("\n");
}

void print_verbose_report(VerboseReport rep){
    printf("f->alt=%d ", rep.alt);
    printf("lv=%d ", rep.layer_values);
    print_report(rep.report);
}
