#include <iostream>
#include <string_view>

int main()
{
	std::string_view sv = "a(b,c)";

	auto p = sv.find('(');
	std::cout << sv.substr(p);
}
