#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main (int a, char** c)
{
	int loop = 1;
	int peg = 1;
	pid_t child_pid = fork();
	switch(child_pid) {
	case 0:
		printf("%d\n", execlp("./zombie", 0));
	case -1:
		loop = 0;
		break;
	default:
		break;
	}
	sleep(60);
	return 0;
}
