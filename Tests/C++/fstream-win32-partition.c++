#include <fstream>
#include <cstdio>
#include <iostream>
#include <iomanip>
int main()
{
	std::fstream disk( "\\\\.\\C:", std::ios::in|std::ios::out|std::ios::binary );
	if(!disk.is_open())
		std::cerr << errno << '\n';
	std::cout << std::hex << std::setfill('0');
	for (int i = 0; i < 32; ++i) {
		std::cout << std::setw(8) << i*16 << ':'; 
		for (int i = 0; i < 16; ++i) {
			char byte;
			disk.read(&byte,1);
			std::cout << ' ' << std::setw(2) << ((unsigned)byte & 0xFF);
		}
		std::cout << '\n';
	}

	std::cin.get();
}
