#include <stdio.h>

int main()
{
	char *hexcode="F";
	int test;
	sscanf(hexcode, "%x", &test);
	printf("%d", test);
	return 0;
}
