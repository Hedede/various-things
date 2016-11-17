#include <stdexcept>

void callback()
{
	throw std::runtime_error("рожа мне твоя не нравится");
}

extern "C" void func(void (*callback)());

#include <iostream>
int main()
{
	try {
		func( callback );
	} catch( std::runtime_error& ex ) {
		std::cerr << ex.what() << '\n';
	}
}
