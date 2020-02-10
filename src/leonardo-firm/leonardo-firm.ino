#include <Arduino.h>
#include <Keyboard.h>
#include "chef.h"
#include "defines.h"
#include "maps.h"
#include "scan.h"

Key keys[KEYS_LEN] = {0};
uint8_t layers[LAYERS_LEN] = {LAYERS_VAL};

void setup(){
    init_gpio();
    Chef chef = {KEYS_LEN, LAYERS_LEN, layers, keys, RETAP_DELAY, HOLD_DELAY};
    chef_init(chef);
    Keyboard.begin();
}

void loop(){
    uint8_t *report = generate_report();
    if(report)
        send_report(report);
}

void send_report(uint8_t *report){
    //Arduino is weird and it "sends" the report.
    //Uses the generated report and sends it to the host
    HID().SendReport(2, report, 8);
}
