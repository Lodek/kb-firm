#include <Arduino.h>
#include "chef.h"
#include "defines.h"
#include "maps.h"
#include "scan.h"


void setup(){
    init_gpio();
}

void loop(){
    _byteint *report;
    update_keys();
    report = generate_report();
    send_report(report);
}

