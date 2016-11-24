#define paiben
#ifndef paiben
#include <cstdio>

int main()
{
	FILE* ffu = fopen("gavish","wb");
	int i = 250000000;
	while(i-- > 0) {
		fwrite(&i,4,1,ffu);
	}
	return 0;
}
#else
#include <fstream>
int main()
{
	std::fstream ffu;
	ffu.open("gavish",std::ios::out | std::ios::binary);
	int i = 250000000;
	while(i-- > 0) {
		// ffu.write((char *)&i,4);
		ffu << i;
	}
	return 0;
}

#endif
