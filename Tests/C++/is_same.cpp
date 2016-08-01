#include <type_traits>

int main()
{
	int i[10][10] = {};
	static_assert(std::is_same<decltype(i), int[10][9]>::value,"");
}
