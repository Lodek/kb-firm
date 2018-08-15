#include <Arduino.h>
#include <Keyboard.h>
#include "chef.h"
#include "defines.h"
#include "maps.h"
#include "scan.h"

HID_ hid;
Key keys[KEYS_LEN] = {0};

void setup(){
    init_gpio();
}

void loop(){
    uint8_t *report;
    report = generate_report();
    if(report)
        send_report(report);
}

void send_report(uint8_t *report){
    //Arduino is weird and it "sends" the report.
    //Uses the generated report and sends it to the host
    HID().SendReport(2, report, 8);
}
