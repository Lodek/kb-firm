#include "scan.h"
#include "maps.h"
#include "defines.h"
#include "chef.h"
#include <Arduino.h>
#include <HID.h>

HID_ hid;

Name names[1][8] = NAMES;
_byteint inputs[INPUTS_LEN] = {INPUT_PINS};
_byteint outputs[OUTPUTS_LEN] = {OUTPUT_PINS};

Key keys[KEYS_LEN] = {0};

void init_gpio(){
    for (_byteint i = 0; i < OUTPUTS_LEN; i++){
        pinMode(outputs[i], OUTPUT);
        digitalWrite(outputs[i], LOW);
    }
    for (_byteint i = 0; i < INPUTS_LEN; i++)
        pinMode(inputs[i], INPUT);
}

void update_keys(){
    for(_byteint i = 0; i < OUTPUTS_LEN; i++){
        digitalWrite(outputs[i], HIGH);
        for(_byteint j = 0; j < INPUTS_LEN; j++){
            int index = i * INPUTS_LEN +j;
            keys[index].active = digitalRead(inputs[j]);
        }
        digitalWrite(outputs[i], LOW);
    }
}

void send_report(_byteint *report){
    //Arduino is weird and it "sends" the report.
    //Uses the generated report and sends it to the host
    hid.SendReport(2, report, 8);
}

_byteint get_trigger(_byteint index, _byteint layer){
    return names[layer][index].trigger;
}
                 
long get_quanta(_byteint index, _byteint layer, _byteint offset){
    return names[layer][index].quantas[offset];
}

                 /*                 
_byteint get_trigger(_byteint index, _byteint layer){
    Name name = pgm_read_word_near(names[layer][index]);
    return name.trigger;
}

long get_quanta(_byteint index, _byteint layer, _byteint offset){
    Name name = pgm_read_word_near(names[layer][index]);
    return name.quantas[offset];
}
                 */
