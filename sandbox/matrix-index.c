#include <stdio.h>

int main()
{
	char a[]="Get";
	char b[]="Memed";	
	char c[]="Son";
	char *arr[3] = {a,b,c};
	for(int i=0; i<3; i++)
		for(int j=0; j<3; j++)
			printf("%c", arr[i][j]);
	return 0;

}
