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
    if(HAS_ANALOG)
        update_analog(keys);
}

void zero_analog(Key* keys){
    for(int i = 0; i < 4; i++)
        keys[KEYS_LEN-4+i].active = 0;
}

void update_analog(Key* keys){
    #define LEFT 0
    #define DOWN 1
    #define UP 2
    #define RIGHT 3

    #define LD 4
    #define LU 5
    #define RU 6
    #define RD 7

    #define LEFT_LOGIC x<200 && y>400 && y<600
    #define RIGHT_LOGIC x>800 && y<600 && y>450
    #define UP_LOGIC x>200 && x<600 && y<200
    #define DOWN_LOGIC x>450 && x<600 && y>900
    #define LU_LOGIC x<200 && y<200
    #define LD_LOGIC x<200 && y>800
    #define RU_LOGIC y<200 && x>800
    #define RD_LOGIC x>900 && y>900
    
    #define COUNTER_NONE 8

    #define PIN_X_AXIS A3
    #define PIN_Y_AXIS A2
    
    #define COUNTER_MAX 50
    
    uint16_t x = analogRead(PIN_X_AXIS);
    uint16_t y = analogRead(PIN_Y_AXIS);

    Serial.begin(9600);
    Serial.print("x= ");
    Serial.println(x);
    Serial.print("y= ");
    Serial.println(y);
    static uint8_t counter[9] = {0};
    uint8_t active, match;

    if(LEFT_LOGIC) counter[LEFT]++;
    else if(LU_LOGIC) counter[LU]++;
    else if(UP_LOGIC) counter[UP]++;
    else if(RU_LOGIC) counter[RU]++;
    else if(RIGHT_LOGIC) counter[RIGHT]++;
    else if(RD_LOGIC) counter[DOWN]++;
    else if(DOWN_LOGIC) counter[DOWN]++;
    else if(LD_LOGIC) counter[LD]++;
    else counter[COUNTER_NONE]++;

    for(int i = 0; i < 9; i++){
        if(counter[i] == COUNTER_MAX){
            for(int j = 0; j < 9; j++)
                counter[j] = 0;
            active = i;
            match = 1;
            break;
        }
        match = 0;
    }
    if(match){
        for(int i = 0; i < 4; i++) keys[KEYS_LEN-4+i].active = 0;
        if(active == LEFT) keys[KEYS_LEN-4+LEFT].active = 1;
        else if(active == DOWN) keys[KEYS_LEN-4+DOWN].active = 1;
        else if(active == RIGHT) keys[KEYS_LEN-4+RIGHT].active = 1;
        else if(active == UP) keys[KEYS_LEN-4+UP].active = 1;
        else if(active == LD){ keys[KEYS_LEN-4+DOWN].active = 1; keys[KEYS_LEN-4+LEFT].active = 1;}
        else if(active == LU){ keys[KEYS_LEN-4+UP].active = 1; keys[KEYS_LEN-4+LEFT].active = 1;}
        else if(active == RD){ keys[KEYS_LEN-4+RIGHT].active = 1; keys[KEYS_LEN-4+DOWN].active = 1;}
        else if(active == RU){ keys[KEYS_LEN-4+UP].active = 1; keys[KEYS_LEN-4+RIGHT].active = 1;}
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
