#include <iostream>
template <typename T>
void print(T const& val)
{
	std::cout << val;
}

int main()
{
	constexpr auto& pi = print<int>;

	pi(22);
}
