#include <stdio.h>
#include <stdint.h>
#include "chef.h"
#include "maps.h"
#include "defines.h"

Key keys[KEYS_LEN] = {0};

void print_verbose_report(VerboseReport rep);
void print_report(uint8_t* rep);

int input_eof = 0;
FILE* file;

int main(){
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
        printf("NULL]\n");
        return;
    }
    for(int i = 0; i < 8-1; i++)
        printf("%d,", rep[i]);
    printf("%d]\n", rep[7]);
}

void print_verbose_report(VerboseReport rep){
    printf("f->alt=%d ", rep.alt);
    printf("lv=%d ", rep.layer_values);
    print_report(rep.report);
}
