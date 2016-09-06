#include<string>

struct Int {
	Int(int i) : i(i) {};
//	operator int() {return i;}
	int i;
};

std::string operator + (std::string ch, Int x)
{
	return ch + std::to_string(x.i);
}

#include <iostream>
int main()
{
	using namespace std::string_literals;
	std::cout << ("" + Int(3));
}
