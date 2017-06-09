#include <stdio.h>
#include <stdlib.h>
#include "hid_keys.h"
#define NUMKEYS 50

int main()
{
  
	int i=0;
    char c;
	int keys[100];
	
	while(cin >> c)
   	{    

	switch(c)
	    {
	    case('!'): keys[i]= HID_S_1
	    case('"'): keys[i]= HID_DOUBLE_QUOTE
	    case('#'): keys[i]= HID_S_3
	    case('$'): keys[i]= HID_S_4
	    case('%'): keys[i]= HID_S_5
	    case('&'): keys[i]= HID_S_7
	    case('\''): keys[i]= HID_SINGLE_QUOTE
	    case('('): keys[i]= HID_S_9
	    case(')'): keys[i]= HID_S_0
	    case('*'): keys[i]= HID_S_8
	    case('+'): keys[i]= HID_PLUS
	    case(','): keys[i]= HID_COMMA
	    case('-'): keys[i]= HID_MINUS
	    case('.'): keys[i]= HID_POINT
	    case('/'): keys[i]= HID_SLASH
	    case('0'): keys[i]= HID_0
	    case('1'): keys[i]= HID_1
	    case('2'): keys[i]= HID_2
	    case('3'): keys[i]= HID_3
	    case('4'): keys[i]= HID_4
	    case('5'): keys[i]= HID_5
	    case('6'): keys[i]= HID_6
	    case('7'): keys[i]= HID_7
	    case('8'): keys[i]= HID_8
	    case('9'): keys[i]= HID_9
	    case(':'): keys[i]= HID_COLON
	    case(';'): keys[i]= HID_SEMICOLON
	    case('<'): keys[i]= HID_LESS
	    case('='): keys[i]= HID_EQUALS
	    case('>'): keys[i]= HID_GREATER
	    case('?'): keys[i]= HID_QUESTION
	    case('@'): keys[i]= HID_S_2
	    case('A'): keys[i]= HID_A
	    case('B'): keys[i]= HID_B
	    case('C'): keys[i]= HID_C
	    case('D'): keys[i]= HID_D
	    case('E'): keys[i]= HID_E
	    case('F'): keys[i]= HID_F
	    case('G'): keys[i]= HID_G
	    case('H'): keys[i]= HID_H
	    case('I'): keys[i]= HID_I
	    case('J'): keys[i]= HID_J
	    case('K'): keys[i]= HID_K
	    case('L'): keys[i]= HID_L
	    case('M'): keys[i]= HID_M
	    case('N'): keys[i]= HID_N
	    case('O'): keys[i]= HID_O
	    case('P'): keys[i]= HID_P
	    case('Q'): keys[i]= HID_Q
	    case('R'): keys[i]= HID_R
	    case('S'): keys[i]= HID_S
	    case('T'): keys[i]= HID_T
	    case('U'): keys[i]= HID_U
	    case('V'): keys[i]= HID_V
	    case('W'): keys[i]= HID_W
	    case('X'): keys[i]= HID_X
	    case('Y'): keys[i]= HID_Y
	    case('Z'): keys[i]= HID_Z
	    case('['): keys[i]= HID_OPENSQRBRACKET
	    case('\\'): keys[i]= HID_BACKSLASH
	    case(']'): keys[i]= HID_CLOSESQRBRACKET
	    case('^'): keys[i]= HID_S_6
	    case('`'): keys[i]= HID_TILDE
	    case('a'): keys[i]= HID_a
	    case('b'): keys[i]= HID_b
	    case('c'): keys[i]= HID_c
	    case('d'): keys[i]= HID_d
	    case('e'): keys[i]= HID_e
	    case('f'): keys[i]= HID_f
	    case('g'): keys[i]= HID_g
	    case('h'): keys[i]= HID_h
	    case('i'): keys[i]= HID_i
	    case('j'): keys[i]= HID_j
	    case('k'): keys[i]= HID_k
	    case('l'): keys[i]= HID_l
	    case('m'): keys[i]= HID_m
	    case('n'): keys[i]= HID_n
	    case('o'): keys[i]= HID_o
	    case('p'): keys[i]= HID_p
	    case('q'): keys[i]= HID_q
	    case('r'): keys[i]= HID_r
	    case('s'): keys[i]= HID_s
	    case('t'): keys[i]= HID_t
	    case('u'): keys[i]= HID_u
	    case('v'): keys[i]= HID_v
	    case('w'): keys[i]= HID_w
	    case('x'): keys[i]= HID_x
	    case('y'): keys[i]= HID_y
	    case('z'): keys[i]= HID_z
	    case('{'): keys[i]= HID_OPENCURLYBRACKET
	    case('|'): keys[i]= HID_PIPE
	    case('}'): keys[i]= HID_OPENCURLYBRACKET
	    case('~'): keys[i]= HID_TILDE_ACCENT
        }
	i++;
	}

	for(int j=0; j<i; j++)
	{
		printf("%#06x, ", keys[j]);
	}

	return 0;
}    
