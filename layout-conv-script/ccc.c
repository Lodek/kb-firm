#include <stdio.h>
#include <stdlib.h>
#define LINESIZE 200
#include <string.h>
#define NUMKEYS 50
#include "hid_keys.h"
void parse_line(char* line, char* latter, char* former, int i);
void printstring(char* str);
void latter_handler(char* latter, int latter_c);
void resetstr(char*);


FILE *in, *out;
int map_counter=0;
int key_map[NUMKEYS]= {0};


int main()
{
  
  int i=0;

  char line[LINESIZE];
  char latter[LINESIZE];
  char former[LINESIZE];

  char scan_var;

  in = fopen("in.txt", "r");
  out = fopen("output.txt", "w");

i=0;

  while((fscanf(in,"%c",&scan_var))==1)
  {

    if((scan_var >=32 && scan_var <=126))
    {
      line[i]=scan_var;
      i++;
    }

    if(scan_var=='\n')
    {
      parse_line(line, latter, former, i);
    }

  }
  fclose(in);
  fclose(out);


  return 0;
}

void printstring(char* str)
{
  for(int i=0; str[i]!='\n'; i++)
  {
    printf("%c", str[i]);
  }
  printf("\n");
   return;
}

void parse_line(char* line, char* latter, char* former, int i)
{
  char temp[LINESIZE];
  int former_c=0, latter_c=0;
  int temp_c=0;
  line[i]='\n';


//removes spaces between \n and the first nontrivial char
  for(int j=i-1; j!=-1; j--)
  {
    if(line[j]!=' ') break;
    line[j]='\n';
    i--;
  }

//reads temp from end to beginning into temp
  for(int j=i-1; j!=-1; j--)
  {
    if(line[j]==' ') break;

    temp[temp_c]=line[j];
    temp_c++;

  }
  temp[temp_c]='\n';
  
//reads temp from end to beginning into latter
  for(int j=temp_c-1; j!=-1; j--)
  {
    latter[latter_c]=temp[j];
    latter_c++;
  }
  latter[latter_c]='\n';

//reads line and adds to former till space
  for(int j=i-temp_c; j>0;)
  {
    if(line[former_c]==' ')
    {
      break;
    }
    former[former_c]=line[former_c];
    former_c++;
  }
    former[former_c]='\n';
    
    printstring(latter);
    printstring(former);
    
    resetstr(line);
    resetstr(former);
    resetstr(latter);
    return;
}

void resetstr(char* str)
{
  for(int i=0; i<LINESIZE; i++)
    str[i]='\0';
  return;
  
}

void latter_handler(char* latter, int latter_c)
{
  if(latter_c==1)
  {
    switch(latter[0])
    {
    case('!'): key_map[map_counter]= HID_S_1
    case('"'): key_map[map_counter]= HID_DOUBLE_QUOTE
    case('#'): key_map[map_counter]= HID_S_3
    case('$'): key_map[map_counter]= HID_S_4
    case('%'): key_map[map_counter]= HID_S_5
    case('&'): key_map[map_counter]= HID_S_7
    case('\''): key_map[map_counter]= HID_SINGLE_QUOTE
    case('('): key_map[map_counter]= HID_S_9
    case(')'): key_map[map_counter]= HID_S_0
    case('*'): key_map[map_counter]= HID_S_8
    case('+'): key_map[map_counter]= HID_PLUS
    case(','): key_map[map_counter]= HID_COMMA
    case('-'): key_map[map_counter]= HID_MINUS
    case('.'): key_map[map_counter]= HID_POINT
    case('/'): key_map[map_counter]= HID_SLASH
    case('0'): key_map[map_counter]= HID_0
    case('1'): key_map[map_counter]= HID_1
    case('2'): key_map[map_counter]= HID_2
    case('3'): key_map[map_counter]= HID_3
    case('4'): key_map[map_counter]= HID_4
    case('5'): key_map[map_counter]= HID_5
    case('6'): key_map[map_counter]= HID_6
    case('7'): key_map[map_counter]= HID_7
    case('8'): key_map[map_counter]= HID_8
    case('9'): key_map[map_counter]= HID_9
    case(':'): key_map[map_counter]= HID_COLON
    case(';'): key_map[map_counter]= HID_SEMICOLON
    case('<'): key_map[map_counter]= HID_LESS
    case('='): key_map[map_counter]= HID_EQUALS
    case('>'): key_map[map_counter]= HID_GREATER
    case('?'): key_map[map_counter]= HID_QUESTION
    case('@'): key_map[map_counter]= HID_S_2
    case('A'): key_map[map_counter]= HID_A
    case('B'): key_map[map_counter]= HID_B
    case('C'): key_map[map_counter]= HID_C
    case('D'): key_map[map_counter]= HID_D
    case('E'): key_map[map_counter]= HID_E
    case('F'): key_map[map_counter]= HID_F
    case('G'): key_map[map_counter]= HID_G
    case('H'): key_map[map_counter]= HID_H
    case('I'): key_map[map_counter]= HID_I
    case('J'): key_map[map_counter]= HID_J
    case('K'): key_map[map_counter]= HID_K
    case('L'): key_map[map_counter]= HID_L
    case('M'): key_map[map_counter]= HID_M
    case('N'): key_map[map_counter]= HID_N
    case('O'): key_map[map_counter]= HID_O
    case('P'): key_map[map_counter]= HID_P
    case('Q'): key_map[map_counter]= HID_Q
    case('R'): key_map[map_counter]= HID_R
    case('S'): key_map[map_counter]= HID_S
    case('T'): key_map[map_counter]= HID_T
    case('U'): key_map[map_counter]= HID_U
    case('V'): key_map[map_counter]= HID_V
    case('W'): key_map[map_counter]= HID_W
    case('X'): key_map[map_counter]= HID_X
    case('Y'): key_map[map_counter]= HID_Y
    case('Z'): key_map[map_counter]= HID_Z
    case('['): key_map[map_counter]= HID_OPENSQRBRACKET
    case('\\'): key_map[map_counter]= HID_BACKSLASH
    case(']'): key_map[map_counter]= HID_CLOSESQRBRACKET
    case('^'): key_map[map_counter]= HID_S_6
    case('`'): key_map[map_counter]= HID_TILDE
    case('a'): key_map[map_counter]= HID_a
    case('b'): key_map[map_counter]= HID_b
    case('c'): key_map[map_counter]= HID_c
    case('d'): key_map[map_counter]= HID_d
    case('e'): key_map[map_counter]= HID_e
    case('f'): key_map[map_counter]= HID_f
    case('g'): key_map[map_counter]= HID_g
    case('h'): key_map[map_counter]= HID_h
    case('i'): key_map[map_counter]= HID_i
    case('j'): key_map[map_counter]= HID_j
    case('k'): key_map[map_counter]= HID_k
    case('l'): key_map[map_counter]= HID_l
    case('m'): key_map[map_counter]= HID_m
    case('n'): key_map[map_counter]= HID_n
    case('o'): key_map[map_counter]= HID_o
    case('p'): key_map[map_counter]= HID_p
    case('q'): key_map[map_counter]= HID_q
    case('r'): key_map[map_counter]= HID_r
    case('s'): key_map[map_counter]= HID_s
    case('t'): key_map[map_counter]= HID_t
    case('u'): key_map[map_counter]= HID_u
    case('v'): key_map[map_counter]= HID_v
    case('w'): key_map[map_counter]= HID_w
    case('x'): key_map[map_counter]= HID_x
    case('y'): key_map[map_counter]= HID_y
    case('z'): key_map[map_counter]= HID_z
    case('{'): key_map[map_counter]= HID_OPENCURLYBRACKET
    case('|'): key_map[map_counter]= HID_PIPE
    case('}'): key_map[map_counter]= HID_OPENCURLYBRACKET
    case('~'): key_map[map_counter]= HID_TILDE_ACCENT
  }

  else
  {
    if((strcmp("SPACE\n", latter) || strcmp("SPC\n", latter)) == 0) key_map[map_counter]=HID_SPACE;

    if((strcmp("RETURN\n", latter) || strcmp("ENTER\n", latter) || strcmp("RET\n", latter)) == 0) key_map[map_counter]=HID_ENTER;

    if((strcmp("BACKSPC\n", latter) || strcmp("BACKSPACE\n", latter)) == 0) key_map[map_counter]=HID_BACKSPACE;

    if((strcmp("TAB\n", latter)) == 0) key_map[map_counter]=HID_TAB;

    if((strcmp("CAPS\n", latter) || strcmp("CAPSLOCK\n", latter)) == 0) key_map[map_counter]=HID_CAPS;

    if((strcmp("PRINT\n", latter) || strcmp("PRINTSCR\n", latter)) == 0) key_map[map_counter]=HID_PRINTSCREEN;

    if((strcmp("SCROLL\n", latter) || strcmp("SCROLLLOCK\n", latter)) == 0) key_map[map_counter]=HID_SCROLLLOCK;

    if((strcmp("PAUSE\n", latter)) == 0) key_map[map_counter]=HID_PAUSE;

    if((strcmp("INSERT\n", latter) || strcmp("INS\n", latter)) == 0) key_map[map_counter]=HID_INSERT;

    if((strcmp("HOME\n", latter)) == 0) key_map[map_counter]=HID_HOME

    if((strcmp("PGUP\n", latter) || strcmp("PAGEUP\n", latter)) == 0) key_map[map_counter]=HID_PAGEUP;

    if((strcmp("DELETE\n", latter) || strcmp("DEL\n", latter)) == 0) key_map[map_counter]=HID_DEL

    if((strcmp("END\n", latter)) == 0) key_map[map_counter]=HID_END;

    if((strcmp("PAGED\n", latter) || strcmp("PAGEDOWN\n", latter)) == 0) key_map[map_counter]=HID_PAGEDOWN;

    if((strcmp("RIGHTARROW\n", latter) || strcmp("RARROW\n", latter)) == 0) key_map[map_counter]=HID_RIGHTARROW;

    if((strcmp("LEFTARROW\n", latter) || strcmp("LARROW\n", latter)) == 0) key_map[map_counter]=HID_LEFTARROW;

    if((strcmp("DOWNARROW\n", latter) || strcmp("DARROW\n", latter)) == 0) key_map[map_counter]=HID_DOWNARROW;

    if((strcmp("UPARROW\n", latter) || strcmp("UARROW\n", latter)) == 0) key_map[map_counter]=HID_UPARROW;

    if((strcmp("NUMLOCK\n", latter) || strcmp("NUM\n", latter)) == 0) key_map[map_counter]=HID_NUMLOCK;

    if((strcmp("RCTRL\n", latter) || strcmp("RCONTROL\n", latter)) == 0) key_map[map_counter]=(1<<12);

    if((strcmp("LCTRL\n", latter) || strcmp("LCONTROL\n", latter) || strcmp("CONTROL\n", latter) || strcmp("CTRL\n", latter)) == 0) key_map[map_counter]=(1<<8);

    if((strcmp("RSHIFT\n", latter)) == 0) key_map[map_counter]=(1<<13);

    if((strcmp("LSHIFT\n", latter) || strcmp("SHIFT\n", latter)) == 0) key_map[map_counter]=(1<<9);

    if((strcmp("RALT\n", latter)) == 0) key_map[map_counter]=(1<<14);

    if((strcmp("LALT\n", latter) || strcmp("ALT\n", latter) == 0) key_map[map_counter]=(1<<10);

    if((strcmp("RGUI\n", latter)) == 0) key_map[map_counter]=(1<<15);
    
    if((strcmp("LGUI\n", latter) || strcmp("GUI\n", latter)) == 0) key_map[map_counter]=(1<<11);

  }
}
