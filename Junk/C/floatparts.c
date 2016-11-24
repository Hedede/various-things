#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char input[4] = {0xC1, 0x70, 0xCD, 0xA4};
unsigned char output[6];

int main(int argc, char** argv)
{
	if (argc > 1) {
		float x = strtof( argv[1], 0 );
		memcpy(input, &x, sizeof(input));
	}

	printf("%x\n", *(unsigned*)input);

	output[0] = (input[3] & 0x80) >> 7;
	output[1] = ((~input[3]) & 0x40) >> 6;
	output[2] = ((input[2] & 0x80) >> 7) + ((input[3] & 0x7f) << 1);
	output[2] -= 127;
	output[3] = (input[0] & 0xff);
	output[4] = (input[1] & 0xff);
	output[5] = (input[2] & 0x7f) | 0x80;
	printf("Sign: %x\nExp:  %d\nMant: %x %x %x\n",
	       output[0], output[1] ? -output[2] : output[2],
	       output[3], output[4], output[5]);
}
