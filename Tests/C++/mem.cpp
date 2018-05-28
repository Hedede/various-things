#include <string>
#include <iostream>
#include <stdexcept>

int main()
{
	constexpr size_t size = 1024*1024;
	size_t i = 0;
	try {
		for (i = 0; i < 10000; ++i) {
			std::string s;
			s.resize(size*i);
		}
	}
	catch(std::exception& ex) {
		std::cerr << "alloc failed at " << size*i << " bytes\n";
		std::cerr << ex.what() << '\n';
	}
}
