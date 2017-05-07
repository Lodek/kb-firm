#include<stdio.h>
#include<stdlib.h>

#define NUMKEYS 50

int main()
{
  
  FILE *in_file;
  FILE *out_file;
  

  in_file = fopen("in.txt", "r");
  out_file = fopen("out.txt", "w");


  char dumb;
  char line[10];
  char
  int line_counter=0;
  int key_map[NUMKEYS]= {0};
  int map_counter=0;

  while(dumb=!EOF)
  {
    for(line_counter=0; dumb!='\n'; line_counter++)
    {
      fscanf("%c",dumb);
      line[line_counter]=dumb;

    }
  

  if(line_counter>1)
  {

    //does stuff to a word
  }

  else
  {
    //all the keys... feelsbad
    switch(line[0])
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

  }
  map_counter++;

  }

  fprintf("{",..);

  for(int j=0; j<map_counter; j++)
  {
    fpritnf("0x%04x, ",_);
    

  }

  fprintf("}");
}  
