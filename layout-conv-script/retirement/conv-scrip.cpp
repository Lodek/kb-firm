#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "hid_keys.h"
using namespace std;

void parse_line(char *line, char **former, char **latter);
int read_line(char *line);
void print_line(char *line, char *former, char *latter);
int mod_handler(char *former);
int hid_switch(char *latter);
int convertor(char *former, char *latter);


FILE *in, *out;
char empty='\0';

int main()
{
    in = fopen("in", "r");
	out = fopen("keys-out", "w");

	char line[20];
	char *former, *latter;
	int ret=1, keycode=0;
	fprintf(out, "{");
    
	while(ret)
    {
		ret=read_line(line);
		if(ret!=0) //if not EOF
		{
			parse_line(line, &former, &latter);
            keycode=convertor(former, latter);
		    fprintf(out, "0x%04x, ", keycode);
		}
		
    }
    fprintf(out, "}");
    fclose(in);
    fclose(out);
	return 0;
}

void parse_line(char *line, char **former, char **latter)
{
	int len=strlen(line);
	*former=&empty;
	*latter=line;
	for(int i=len-1; i>0; i--)
	{
		if(line[i]==' ')
		{
			line[i]='\0';
			*latter=&line[i]+1;
			*former=line;
		}
	}
	
	return; 
}

int read_line(char *line)
{

	int stop_sign=1, ret=1, i=0;
	char scan_var=0;
    while(scan_var!='\n')
	{
		ret=fscanf(in, "%c", &scan_var);
		if(ret==EOF)
		{
			stop_sign=0;
			return stop_sign;
		}
		if(scan_var=='\n') line[i]='\0';
		else line[i]=scan_var;
		i++;
	}
	return stop_sign;
}

void print_line(char *line, char *former, char *latter)
{
	cout << "line " << line << "\n former: " << former << "\n lattter: " << latter << '\n';
	return;
}

int convertor(char *former, char *latter)
{
	int code_latter, code_former, keycode;

	code_latter=hid_switch(latter);	
	code_former=mod_handler(former);
	keycode= code_latter|code_former;
	return keycode;
}

int hid_switch(char *latter)
{
	int ok=0;
	if(strlen(latter)==1)
	{
      switch(latter[0])
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
	}
	else
	{
	    if((strcmp("SPACE", latter) || strcmp("SPC", latter)) == 0) ok=HID_SPACEBAR;
	
	    if((strcmp("RETURN", latter) || strcmp("ENTER", latter) || strcmp("RET", latter)) == 0) ok=HID_ENTER;
	
	    if((strcmp("BACKSPC", latter) || strcmp("BACKSPACE", latter)) == 0) ok=HID_BACKSPACE;
	
	    if((strcmp("TAB", latter)) == 0) ok=HID_TAB;
	
	    if((strcmp("CAPS", latter) || strcmp("CAPSLOCK", latter)) == 0) ok=HID_CAPSLOCK;
	
	    if((strcmp("PRINT", latter) || strcmp("PRINTSCR", latter)) == 0) ok=HID_PRINTSCREEN;
	
	    if((strcmp("SCROLL", latter) || strcmp("SCROLLLOCK", latter)) == 0) ok=HID_SCROLLLOCK;
	
	    if((strcmp("PAUSE", latter)) == 0) ok=HID_PAUSE;
	
	    if((strcmp("INSERT", latter) || strcmp("INS", latter)) == 0) ok=HID_INSERT;
	
	    if((strcmp("HOME", latter)) == 0) ok=HID_HOME;
	
	    if((strcmp("PGUP", latter) || strcmp("PAGEUP", latter)) == 0) ok=HID_PAGEUP;
	
	    if((strcmp("DELETE", latter) || strcmp("DEL", latter)) == 0) ok=HID_DELETE;
	
	    if((strcmp("END", latter)) == 0) ok=HID_END;
	
	    if((strcmp("PAGED", latter) || strcmp("PAGEDOWN", latter)) == 0) ok=HID_PAGEDOWN;
	
	    if((strcmp("RIGHTARROW", latter) || strcmp("RARROW", latter)) == 0) ok=HID_RIGHTARROW;
	
	    if((strcmp("LEFTARROW", latter) || strcmp("LARROW", latter)) == 0) ok=HID_LEFTARROW;
	
	    if((strcmp("DOWNARROW", latter) || strcmp("DARROW", latter)) == 0) ok=HID_DOWNARROW;
	
	    if((strcmp("UPARROW", latter) || strcmp("UARROW", latter)) == 0) ok=HID_UPARROW;
	
	    if((strcmp("NUMLOCK", latter) || strcmp("NUM", latter)) == 0) ok=HID_NUMLOCK;
	
	    if((strcmp("RCTRL", latter) || strcmp("RCONTROL", latter)) == 0) ok=(1<<12);
	
	    if((strcmp("LCTRL", latter) || strcmp("LCONTROL", latter) || strcmp("CONTROL", latter) || strcmp("CTRL", latter)) == 0) ok=(1<<8);
	
	    if((strcmp("RSHIFT", latter)) == 0) ok=(1<<13);
	
	    if((strcmp("LSHIFT", latter) || strcmp("SHIFT", latter)) == 0) ok=(1<<9);
	
	    if((strcmp("RALT", latter)) == 0) ok=(1<<14);
	
	    if((strcmp("LALT", latter)) || strcmp("ALT", latter) == 0) ok=(1<<10);
	
	    if((strcmp("RGUI", latter)) == 0) ok=(1<<15);
	    
	    if((strcmp("LGUI", latter) || strcmp("GUI", latter)) == 0) ok=(1<<11);

	}
	return ok;
}

int mod_handler(char *former)
{
	int ok=0, len=strlen(former);
	if(*former=='\0') return ok;

	for(int i=0; i<len; i++)
	{
		switch(former[i])
		{
			case 'c': case 'C': ok = ok|(1<<8); break;
			case 'a': case 'A': ok = ok|(1<<10); break;
			case 'g': case 'G': ok = ok|(1<<11); break;
			case 's': case 'S': ok = ok|(1<<9); break;
			defaut: ;
		}
	}
	return ok;
}
