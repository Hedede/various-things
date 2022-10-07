#include <iostream>

struct lol {
	unsigned x : 8 = 0x12;
	unsigned y : 8 = 0x34;
	unsigned z : 8 = 0x56;
	unsigned c : 8 = 0x78;
};

int main()
{
	lol l;
	std::cout << std::hex << *(unsigned*)&l << '\n';
}
