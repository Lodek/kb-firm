#include <stdio.h>

int main()
{
	char *hexcode="0xF";
	int test;
	sscanf(hexcode, "%x", &test);
	printf("%d", test);
	return 0;
}
