#include <experimental/string_view>

void func(std::string);

int main()
{
	using namespace std::experimental;
	func(string_view("a",1));
}
