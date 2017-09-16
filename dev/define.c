#include <stdio.h>
#include "define.h"

int main()
{
	int array[2] = {LAYER};
	
	for(int i=0; i<3; i++)
		for(int j=0; j<0; j++) printf("%d", array[i][j]);
	return 0;	

}
