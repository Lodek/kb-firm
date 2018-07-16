#include <stdio.h>
#include <iostream>

using namespace std;

typedef struct
{
	int i,j;
} testruct;

int main()
{
	testruct hold;
	hold.i=1;
	hold.j=2;
	
	long struct_address;
	struct_address= (long) &hold;

	testruct *pointertest = (testruct *) struct_address;
	
	cout << pointertest->i << "\n";
	cout << pointertest->j << "\n";
	return 0;
}

