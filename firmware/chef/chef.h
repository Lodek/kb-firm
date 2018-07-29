#include "defines.h"

//Defines the meaning for quanta's 4th byte
typedef enum {normal, toggle, macro, dead, absolute} quanta_type;

//Possible triggers for a physical key switch.
typedef enum {common, hold, doubletap, dth} key_triggers;

typedef char _byteint;
typedef long _long;

typedef struct {
    //active -> indicate whether key is pressed or not
    //handled -> key has been dealt with
    //remove -> marked for removal
    //toggled -> aux bit to handle toggles
    _byteint active, handled, remove, toggled;
    _byteint written_mod, written_layer, report_index;
    _byteint index, trigger;
    //Conceptually, Key should have a Name for each layer.
    //Due to hardware limitations however (ie runtime SRAM), Names were moved to ROM,
    //thus it was separated from Key since Key contains state variables.
    //It is helpful to think of Key as containing Names.
} Key;

typedef struct {
    //Keys are complicated beings, they have various names, each of which contains bits of its existence.
    //Each name defines what that key does in that layer.
    //Name contains that key's Quantas and triggers for that layer.
    key_triggers trigger;
    long quantas[1];
} Name;

_byteint* generate_report();
void key_handler(Key* key);
void trigger_common(Key* key);
void trigger_hold(Key* key);
void trigger_dtap(Key* key);
void trigger_dth(Key* key);
void base_triple_trigger(Key* key, long tap_quanta, long dtap_quanta, long hold_quanta);
void quanta_handler(Key* key, long quanta);
_byteint report_append(Key* key, _byteint layer, _byteint mod, _byteint keycode);
_byteint report_pop(Key* key);
void layer_mapper();
void gen_hash();
_byteint hash_diff();

extern Key keys[];
