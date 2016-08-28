#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
char const* print_size(size_t size)
{
	static __thread char buffer[64];
	char const* suffixes[] = { "B", "KB", "MB", "GB", "TB", "PB", "EB" };

	size_t i = 0;
	while (size >= 1024*1024) {
		size >>= 10;
		++i;
	}

	double val = size;

	if (size >= 1024) {
		val /= 1024.0;
		++i;
	}

	snprintf( buffer, sizeof(buffer), "%.3f %s", val, suffixes[i]);
	return buffer;
}


#include <assert.h>
#include <string.h>
int main(int c, char const* argv[])
{
	assert(argv[1] != 0);
	size_t b = atol(argv[1]);

	puts(print_size(b));
}
