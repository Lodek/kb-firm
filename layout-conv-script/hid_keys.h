#define MOD_CONTROL_LEFT    (1<<0)
#define MOD_SHIFT_LEFT      (1<<1)
#define MOD_ALT_LEFT        (1<<2)
#define MOD_GUI_LEFT        (1<<3)
#define MOD_CONTROL_RIGHT   (1<<4)
#define MOD_SHIFT_RIGHT     (1<<5)
#define MOD_ALT_RIGHT       (1<<6)
#define MOD_GUI_RIGHT       (1<<7)

#define HID_a     0x4
#define HID_b     0x5
#define HID_c     0x6
#define HID_d     0x7
#define HID_e     0x8
#define HID_f     0x9
#define HID_g      0xA
#define HID_h      0xB
#define HID_i      0xC
#define HID_j      0xD
#define HID_k      0xE
#define HID_l      0xF
#define HID_m      0x10
#define HID_n      0x11
#define HID_o      0x12
#define HID_p      0x13
#define HID_q      0x14
#define HID_r      0x15
#define HID_s      0x16
#define HID_t      0x17
#define HID_u      0x18
#define HID_v      0x19
#define HID_w      0x1A
#define HID_x      0x1B
#define HID_y      0x1C
#define HID_z      0x1D

#define HID_A     0x2004
#define HID_B     0x2005
#define HID_C     0x2006
#define HID_D     0x2007
#define HID_E     0x2008
#define HID_F     0x2009
#define HID_G      0x200A
#define HID_H      0x200B
#define HID_I      0x200C
#define HID_J      0x200D
#define HID_K      0x200E
#define HID_L      0x200F
#define HID_M      0x2010
#define HID_N      0x2011
#define HID_O      0x2012
#define HID_P      0x2013
#define HID_Q      0x2014
#define HID_R      0x2015
#define HID_S      0x2016
#define HID_T      0x2017
#define HID_U      0x2018
#define HID_V      0x2019
#define HID_W      0x201A
#define HID_X      0x201B
#define HID_Y      0x201C
#define HID_Z      0x201D


//NUMROW
#define HID_1       0x1E
#define HID_2       0x1F
#define HID_3       0x20
#define HID_4       0x21
#define HID_5       0x22
#define HID_6       0x23
#define HID_7       0x24
#define HID_8       0x25
#define HID_9       0x26
#define HID_0       0x27

//NUMROW SHIFT
#define HID_S_1       0x201E
#define HID_S_2       0x201F
#define HID_S_3       0x2020
#define HID_S_4       0x2021
#define HID_S_5       0x2022
#define HID_S_6       0x2023
#define HID_S_7       0x2024
#define HID_S_8       0x2025
#define HID_S_9       0x2026
#define HID_S_0       0x2027
 
//Punctuation

#define HID_MINUS 0x2D
#define HID_EQUALS 0x2E
#define HID_OPENSQRBRACKET 0x2F
#define HID_CLOSESQRBRACKET 0x30
#define HID_BACKSLASH 0x31
#define HID_NONUS_HASH 0x32
#define HID_SEMICOLON 0x33
#define HID_SINGLE_QUOTE 0x34
#define HID_TILDE 0x35
#define HID_COMMA 0x36
#define HID_POINT 0x37
#define HID_SLASH 0x38
#define HID_UNDERSCORE 0x202D
#define HID_PLUS 0x202E
#define HID_OPENCURLYBRACKET 0x202F
#define HID_CLOSECURLYBRACKET 0x2030
#define HID_PIPE 0x2031
#define HID_NONUS_NOT 0x2032
#define HID_COLON 0x2033
#define HID_DOUBLE_QUOTE 0x2034
#define HID_TILDE_ACCENT 0x2035
#define HID_LESS 0x2036
#define HID_GREATER 0x2037
#define HID_QUESTION 0x2038

// HID Values of Function Keys
#define HID_F1 0x3a
#define HID_F2 0x3b
#define HID_F3 0x3c
#define HID_F4 0x3d
#define HID_F5 0x3e
#define HID_F6 0x3f
#define HID_F7 0x40
#define HID_F8 0x41
#define HID_F9 0x42
#define HID_F10 0x43
#define HID_F11 0x44
#define HID_F12 0x45


//Word keys
#define HID_ENTER 0x28
#define HID_ESCAPE 0x29
#define HID_BACKSPACE 0x2a
#define HID_TAB 0x2b
#define HID_SPACEBAR 0x2c


#define HID_CAPSLOCK 0x39
#define HID_PRINTSCREEN 0x46
#define HID_SCROLLLOCK 0x47
#define HID_PAUSE 0x48
#define HID_INSERT 0x49
#define HID_HOME 0x4a
#define HID_PAGEUP 0x4b
#define HID_DELETE 0x4c
#define HID_END 0x4d
#define HID_PAGEDOWN 0x4e
#define HID_RIGHTARROW 0x4f
#define HID_LEFTARROW 0x50 
#define HID_DOWNARROW 0x51
#define HID_UPARROW 0x52

// HID Values of Keypad Keys
#define HID_NUMLOCK 0x53
#define HID_KEYPADSLASH 0x54
#define HID_KEYPADSTAR 0x55
#define HID_KEYPADMINUS 0x56
#define HID_KEYPADPLUS 0x57
#define HID_KEYPADENTER 0x58
#define HID_KEYPAD1 0x59
#define HID_KEYPAD2 0x5a
#define HID_KEYPAD3 0x5b
#define HID_KEYPAD4 0x5c
#define HID_KEYPAD5 0x5d
#define HID_KEYPAD6 0x5e
#define HID_KEYPAD7 0x5f
#define HID_KEYPAD8 0x60
#define HID_KEYPAD9 0x61
#define HID_KEYPAD0 0x62
#define HID_KEYPADPERIOD 0x63

//special
#define HID_MUTE 0x7f
#define HID_VOLUP 0x80
#define HID_VOLDOWN 0x81
