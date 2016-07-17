#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

#include <unistd.h>

#include <stdio.h>

/*
 * int Add( int a, int b )
 * {
 * 	return a + b;
 * 	
 * }
 */

static char const asm[] = {
	0x8d, 0x04, 0x37,	// lea		(%rdi,%rsi,1),%eax
	0xc3			// retq
};


int main (int argc, char **argv)
{
	int fd = open("not-a-function.bin", O_RDONLY);
	if (fd < 0) {
		fd = open("not-a-function.bin", O_RDWR|O_CREAT|O_TRUNC, 0755);
		if (fd < 1)
			return -1;
		write(fd, (void const*)asm, sizeof(asm));
	}

	struct stat st;
	int ret = fstat(fd, &st);
	if (ret < 0)
		return ret;

	void* mem;
	mem = mmap(NULL, (long)st.st_size, 
			PROT_READ | PROT_EXEC, 
			MAP_SHARED, fd, 0);
	if ((long)mem == -1)
		return -1;

	typedef int (*funcptr)(int, int);

	funcptr add = (funcptr)mem;
	printf("%d", add(2,3));

	return 0;
}
