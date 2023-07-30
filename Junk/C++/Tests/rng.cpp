#include <random>
#include <iostream>
std::random_device dev;
int main()
{
	std::cout << dev() << '\n';
}
