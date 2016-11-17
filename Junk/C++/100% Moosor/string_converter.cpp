#include <aw/utility/to_string/pair.h>
#include <iostream>
namespace aw {
void func()
{
	aw::format::pretty_print fmt{};
	std::cout << string_converter<std::pair<int,int>>{std::make_pair(0,0)}(fmt);
	std::cout << to_string(std::make_pair(0,0));
}
}

int main() { aw::func(); }
