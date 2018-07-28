#include <stdio.h>

typedef struct{
    int k;
} s2;
    
typedef struct{
	int i;
    s2 t[2];
} s1;

int main()
{
    s1 test = {0, {{1},{2}}};
    printf("%d, %d, %d",test.i, test.t[0].k, test.t[1].k);
	return 0;
}

