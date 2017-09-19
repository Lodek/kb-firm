void start_serial(); //init serial comms
void start_gpio(); //sets up the pins in the arduino
void start_keys();//maps the mapping array to the key structs
void start_mapping();//maps keys using mappings matrix

void start_layer(char**map, int current_layer);
void start_l0();
void start_l1();
void start_l2();
