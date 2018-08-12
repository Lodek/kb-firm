#include <stdio.h>

typedef struct{
	int i;
    int const *t;
} s1;

int main()
{
    s1 test = {0};
    test.t = (int const[]){0,1};
    printf("%d, %d", test.t[0], test.t[1]);
	return 0;
}

