#include <cstdint>
using std::uint32_t;

struct Vector3d{
	Vector3d(float x, float y, float z)
		: c{x,y,z}
	{ }

	float c[3];
};

int digit(char c)
{
	switch(c) {
	case '0':return 0;
	case '1':return 1;
	case '2':return 2;
	case '3':return 3;
	case '4':return 4;
	case '5':return 5;
	case '6':return 6;
	case '7':return 7;
	case '8':return 8;
	case '9':return 9;
	case 'a':return 0xa;
	case 'b':return 0xb;
	case 'c':return 0xc;
	case 'd':return 0xd;
	case 'e':return 0xe;
	case 'f':return 0xf;
	}
}

char tigid(uint32_t c)
{
	switch(c) {
	case 0x0:return '0';
	case 0x1:return '1';
	case 0x2:return '2';
	case 0x3:return '3';
	case 0x4:return '4';
	case 0x5:return '5';
	case 0x6:return '6';
	case 0x7:return '7';
	case 0x8:return '8';
	case 0x9:return '9';
	case 0xa:return 'a';
	case 0xb:return 'b';
	case 0xc:return 'c';
	case 0xd:return 'd';
	case 0xe:return 'e';
	case 0xf:return 'f';
	}
}

#include <iostream>

struct Bignum {
	Bignum(std::initializer_list<uint8_t> cl)
		: intz{0}
	{
		int curnum = 0;
		int curdig = 1;
		for (auto& i : intz) {
			i = 0;
		}
		for (auto c : cl) {
			intz[curnum] += digit(c) * curdig;
			if (curdig == 1000'000'000) {
				curdig = 1;
				curnum++;
				continue;
			}
			curdig *= 10;
		}
		used = curnum+1;
	}

	std::string print()
	{
		std::string out;
		for (int i = 0; i < used; ++i) {
			out += tigid((intz[i] >> 0x00) & 0xf);
			out += tigid((intz[i] >> 0x04) & 0xf);
			out += tigid((intz[i] >> 0x08) & 0xf);
			out += tigid((intz[i] >> 0x0C) & 0xf);
			out += tigid((intz[i] >> 0x10) & 0xf);
			out += tigid((intz[i] >> 0x14) & 0xf);
			out += tigid((intz[i] >> 0x18) & 0xf);
			out += tigid((intz[i] >> 0x1C) & 0xf);
		}
		if (!used)
			return "0";
		return out;
	}

	bool sign;
	unsigned int intz[63];
	unsigned int used = 0;
};

template <char... digits>
Bignum
operator "" _fu ()
{
	return Bignum({digits..., 0});
}

int main()
{
	std::cout << 10_fu .print();
}
