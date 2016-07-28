#include <stdio.h>

int main(int a, char** v)
{
	char buffer[8192];
	setvbuf(stdout, buffer, _IOFBF, sizeof(buffer));

	for(int i=1; i < 100; i++) {
		printf("%d\n",i);
	}

	return -1;
}
