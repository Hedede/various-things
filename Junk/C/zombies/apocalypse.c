#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main (int a, char** c)
{
	int loop = 1;
	int peg = 1;
	while(loop) {
		pid_t child_pid = fork();
		switch(child_pid) {
		case 0:
			exit(0);
		case -1:
			loop = 0;
			break;
		default:
			if(peg) {
				peg = 0;
				printf("It's ZOMBIE APOCALYPSE! Everybody shut doors!\n");
			}
			break;
		}
	}
	sleep(60);
	return 0;
}
