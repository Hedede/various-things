#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char symbol[] = {
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
	'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
	'U', 'V', 'W', 'X', 'Y', 'Z'
};

int main(int argc, char** argv)
{
	assert(argv[1] != 0);
	assert(argv[2] != 0);

	size_t b = atol(argv[1]);
	size_t q = atol(argv[2]);

	assert(b > 1 && b < 37);

	char r[65];

	memset(r, 0, 65);

	char* tmp = r;
	while (q >= b) {
		*(tmp++) = symbol[q % b];
		q /= b;
	}

	*tmp = symbol[q];

	while (tmp >= r)
		putchar(*(tmp--));

	putchar('\n');
}
