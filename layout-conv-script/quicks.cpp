#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "hid_keys.h"
#define NUMKEYS 50

using namespace std;

int main()
{
  
    char c;
	int ok;
	while(c!='@')
   	{    
	  cin >> c;
      switch(c)
	    {
        case '!' : ok= HID_S_1; break;
	    case '"' : ok= HID_DOUBLE_QUOTE; break;
	    case '#' : ok= HID_S_3; break;
	    case '$' : ok= HID_S_4; break;
	    case '%' : ok= HID_S_5; break;
	    case '&' : ok= HID_S_7; break;
	    case '\'' : ok= HID_SINGLE_QUOTE; break;
	    case '(' : ok= HID_S_9; break;
	    case ')' : ok= HID_S_0; break;
	    case '*' : ok= HID_S_8; break;
	    case '+' : ok= HID_PLUS; break;
	    case ',' : ok= HID_COMMA; break;
	    case '-' : ok= HID_MINUS; break;
	    case '.' : ok= HID_POINT; break;
	    case '/' : ok= HID_SLASH; break;
	    case '0' : ok= HID_0; break;
	    case '1' : ok= HID_1; break;
	    case '2' : ok= HID_2; break;
	    case '3' : ok= HID_3; break;
	    case '4' : ok= HID_4; break;
	    case '5' : ok= HID_5; break;
	    case '6' : ok= HID_6; break;
	    case '7' : ok= HID_7; break;
	    case '8' : ok= HID_8; break;
	    case '9' : ok= HID_9; break;
	    case ':' : ok= HID_COLON; break;
	    case ';' : ok= HID_SEMICOLON; break;
	    case '<' : ok= HID_LESS; break;
	    case '=' : ok= HID_EQUALS; break;
	    case '>' : ok= HID_GREATER; break;
	    case '?' : ok= HID_QUESTION; break;
	    case '@' : ok= HID_S_2; break;
	    case 'A' : ok= HID_A; break;
	    case 'B' : ok= HID_B; break;
	    case 'C' : ok= HID_C; break;
	    case 'D' : ok= HID_D; break;
	    case 'E' : ok= HID_E; break;
	    case 'F' : ok= HID_F; break;
	    case 'G' : ok= HID_G; break;
	    case 'H' : ok= HID_H; break;
	    case 'I' : ok= HID_I; break;
	    case 'J' : ok= HID_J; break;
	    case 'K' : ok= HID_K; break;
	    case 'L' : ok= HID_L; break;
	    case 'M' : ok= HID_M; break;
	    case 'N' : ok= HID_N; break;
	    case 'O' : ok= HID_O; break;
	    case 'P' : ok= HID_P; break;
	    case 'Q' : ok= HID_Q; break;
	    case 'R' : ok= HID_R; break;
	    case 'S' : ok= HID_S; break;
	    case 'T' : ok= HID_T; break;
	    case 'U' : ok= HID_U; break;
	    case 'V' : ok= HID_V; break;
	    case 'W' : ok= HID_W; break;
	    case 'X' : ok= HID_X; break;
	    case 'Y' : ok= HID_Y; break;
	    case 'Z' : ok= HID_Z; break;
	    case '[' : ok= HID_OPENSQRBRACKET; break;
	    case '\\' : ok= HID_BACKSLASH; break;
	    case ']' : ok= HID_CLOSESQRBRACKET; break;
	    case '^' : ok= HID_S_6; break;
	    case '`' : ok= HID_TILDE; break;
	    case 'a' : ok= HID_a; break;
	    case 'b' : ok= HID_b; break;
	    case 'c' : ok= HID_c; break;
	    case 'd' : ok= HID_d; break;
	    case 'e' : ok= HID_e; break;
	    case 'f' : ok= HID_f; break;
	    case 'g' : ok= HID_g; break;
	    case 'h' : ok= HID_h; break;
	    case 'i' : ok= HID_i; break;
	    case 'j' : ok= HID_j; break;
	    case 'k' : ok= HID_k; break;
	    case 'l' : ok= HID_l; break;
	    case 'm' : ok= HID_m; break;
	    case 'n' : ok= HID_n; break;
	    case 'o' : ok= HID_o; break;
	    case 'p' : ok= HID_p; break;
	    case 'q' : ok= HID_q; break;
	    case 'r' : ok= HID_r; break;
	    case 's' : ok= HID_s; break;
	    case 't' : ok= HID_t; break;
	    case 'u' : ok= HID_u; break;
	    case 'v' : ok= HID_v; break;
	    case 'w' : ok= HID_w; break;
	    case 'x' : ok= HID_x; break;
	    case 'y' : ok= HID_y; break;
	    case 'z' : ok= HID_z; break;
	    case '{' : ok= HID_OPENCURLYBRACKET; break;
	    case '|' : ok= HID_PIPE; break;
	    case '}' : ok= HID_OPENCURLYBRACKET; break;
	    case '~' : ok= HID_TILDE_ACCENT; break;
        }
      printf("0x%04x, ", ok);
	}



	return 0;
}    
