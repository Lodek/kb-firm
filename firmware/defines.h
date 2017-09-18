#define NUM_COLL 13
#define NUM_ROW 4
#define NUM_PINS NUM_ROW+NUM_COLL
#define NUM_KEYS NUM_COLL*NUM_ROW
#define NUM_LAYERS 2
#define LAYER_VALUE 0,1

#define HOLD_SLEEP 150

#define DOUBLETAP_TAP_DELAY  100
#define DOUBLETAP_RELEASE_DELAY 100

#define DTAPHOLD_RELEASE_DELAY 100
#define DTAPHOLD_TAP_DELAY  100

//csv notation
#define ROW_PINS 1,2,3,4
#define COL_PINS 21, 20, 19, 18, 15, 14, 16, 10, 9, 8, 7, 6, 5

#define LAYERS L1,L2,L3
#define L0 "0x000000000x0000002b","0x000000000x00000014","0x000000000x0000001a","0x000000000x00000008","0x000000000x00000015","0x000000000x00000017","0x000000000x0000001c","0x000000000x00000018","0x000000000x0000000c","0x000000000x00000012","0x000000000x00000013","0x000000000x0000002a","0x000000000x0000002a","0x000000000x00000029","0x000000000x00000004","0x000000000x00000016","0x000000000x00000007","0x000000000x00000009","0x000000000x0000000a","0x000000000x0000000b","0x000000000x0000000d","0x000000000x0000000e","0x000000000x0000000f","0x000000000x00000033","0x000000000x00000034","0x000000000x00000028","0x000000000x00000200","0x000000000x0000001d","0x000000000x0000001b","0x000000000x00000006","0x000000000x00000019","0x000000000x00000005","0x000000000x00000011","0x000000000x00000010","0x000000000x00000036","0x000000000x00000037","0x000000000x00000038","0x000000000x00000100","0x000000000x00000028","0x000000000x00000100","0x000000000x00000800","0x000000000x00000400","0x000000000x00000000","0x000010000x00000000","0x000000000x0000002c","0x000000000x0000002c","0x000020000x00000000","0x000000000x00000000","0x000000000x00000000","0x000000000x00000000","0x000000000x00000100","0x000000000x0000004c"
#define L1 "0x000000000x00000037","0x000000000x00000024","0x000000000x00000025","0x000000000x00000026","0x000000000x00002024","0x000000000x00002025","0x000000000x0000002d","0x000000000x00000031","0x000000000x00000038","0x000000000x00002026","0x000000000x00002027","0x000000000x0000002a","0x000000000x0000002a","0x000000000x00000027","0x000000000x00000021","0x000000000x00000022","0x000000000x00000023","0x000000000x00002021","0x000000000x00002022","0x000000000x00002023","0x000000000x00002036","0x000000000x00002037","0x000000000x0000202f","0x000000000x00002030","0x000000000x00002031","0x000000000x00000028","0x000000000x00000200","0x000000000x0000001e","0x000000000x0000001f","0x000000000x00000020","0x000000000x0000201e","0x000000000x0000201f","0x000000000x00002020","0x000000000x0000002e","0x000000000x00000035","0x000000000x0000002f","0x000000000x00000030","0x000000000x00000200","0x000000000x00000028","0x000000000x00000100","0x000000000x00000800","0x000000000x00000400","0x000000000x00000000","0x000000000x00010000","0x000000000x0000002a","0x000000000x0000002a","0x000020000x00000000","0x000000000x00000000","0x000000000x00000000","0x000000000x00000000","0x000000000x00000100","0x000000000x0000004c"
#define L2 "0x000000000x0000003a","0x000000000x0000003b","0x000000000x0000003c","0x000000000x0000003d","0x000000000x0000003e","0x000000000x0000003f","0x000000000x00000040","0x000000000x00000041","0x000000000x00000042","0x000000000x00000043","0x000000000x00000044","0x000000000x0000002a","0x000000000x0000002a","0x000000000x00000039","0x000000000x00000000","0x000000000x00000000","0x000000000x00000000","0x000000000x00000000","0x000000000x00000000","0x000000000x00000050","0x000000000x00000051","0x000000000x00000052","0x000000000x0000004f","0x000000000x00000000","0x000000000x00000045","0x000000000x00000028","0x000000000x00000200","0x000000000x00000000","0x000000000x00000000","0x000000000x00000000","0x000000000x00000000","0x000000000x00000000","0x000000000x0000004a","0x000000000x0000004b","0x000000000x0000004e","0x000000000x0000004d","0x000000000x00000046","0x000000000x00000200","0x000000000x00000028","0x000000000x00000100","0x000000000x00000800","0x000000000x00000400","0x000000000x00000000","0x000000000x00010000","0x000000000x00000028","0x000000000x00000028","0x000020000x00000000","0x000000000x00000000","0x000000000x00000000","0x000000000x00000000","0x000000000x00000100","0x000000000x0000002c"