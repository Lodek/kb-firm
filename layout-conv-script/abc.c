#include <stdio.h>
#include <stdlib.h>
#define LINESIZE 200

void printstring(char* str);

int main()
{
  
  int former_c=0, latter_c=0, i=0;
  FILE *in, *out;
  char line[LINESIZE];
  char latter[LINESIZE];
  char former[LINESIZE];
  char temp[LINESIZE];
  char scan_var;
  int temp_c=0;
  int u;

  in = fopen("in.txt", "r");
  out = fopen("output.txt", "w");

temp_c=0;
former_c=0;
latter_c=0;
i=0;

 while(scan_var!='\n')
    {
       fscanf(in, "%c", &scan_var);

      if((scan_var >=32 && scan_var <=126))
        {
          line[i]=scan_var;
          i++;
        }
    }

  line[i]='\n';
  printstring(line);

//removes spaces between \n and the first nontrivial char
  for(int j=i-1; j!=-1; j--)
  {
    if(line[j]!=' ') break;
    line[j]='\n';
    i--;
  }
  printstring(line);
//reads temp from end to beginning into temp
  for(int j=i-1; j!=-1; j--)
  {
    if(line[j]==' ') break;

    temp[temp_c]=line[j];
    temp_c++;

  }
  temp[temp_c]='\n';
  
  printstring(temp);
  
//reads temp from end to beginning into latter
  for(int j=temp_c-1; j!=-1; j--)
  {
    latter[latter_c]=temp[j];
    latter_c++;
  }
  latter[latter_c]='\n';
  printstring(latter);

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
  printstring(former);


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
