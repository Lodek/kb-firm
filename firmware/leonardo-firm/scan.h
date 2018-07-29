
typedef char _byteint;

void init_gpio();
void update_keys();
void send_report(_byteint *report);
_byteint get_trigger(_byteint index, _byteint layer);
long get_quanta(_byteint index, _byteint layer, _byteint offset);
