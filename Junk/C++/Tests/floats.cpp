#include <iostream>
#include <bitset>

unsigned short bs(float f)
{
	unsigned short v = (long long)(f*256.0);
	return v;
}

int main()
{
	for (float f = -127.0; f < 127.0; f += 32)
	{
		auto b = bs(f);
		std::bitset<16> bits(b);
		std::cout << f << ' ' << b << ' ' << bits << ' ' << ((signed short)(b)*0.00390625) << '\n';
	}
}
