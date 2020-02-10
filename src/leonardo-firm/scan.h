#include <stdint.h>
#include "chef.h"

void update_keys(Key* keys);
void init_gpio();
uint8_t get_trigger(int index, int layer);
uint32_t get_quanta(int index, int layer, int offset);
void update_analog(Key* keys);
void zero_analog(Key* keys);
