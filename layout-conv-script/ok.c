#include<stdio.h>
#include<stdlib.h>

#define NUMKEYS 50
int main()
{
  
  FILE *in_file;
  FILE *out_file;
  char c=0;

  out_file = fopen("out.txt", "w");



    
  for(int i=0; i<255; i++)
    {
            c=i;
            fprintf(out_file, "%d %c\n", c,c);
      
    }

  return 0;
  

}  
