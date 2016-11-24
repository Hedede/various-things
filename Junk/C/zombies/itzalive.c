#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main (int a, char** c)
{
	pid_t child_pid = fork();
	if (child_pid > 0) {
		printf("IT'S ALIVE!");
		sleep (60);
	} else {
		exit (0);
	}
	return 0;
}
