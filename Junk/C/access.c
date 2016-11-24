#include <stdio.h>
#include <unistd.h>

int main ()
{
	printf("%d\n",access("Downloads", R_OK));
	return 0;
}
