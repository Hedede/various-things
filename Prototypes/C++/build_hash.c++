constexpr const char build_date[] = __DATE__ " " __TIME__;

template<unsigned N>
constexpr unsigned crude_hash(char const (&string) [N])
{
	unsigned i = 1;
	for (auto c : string) {
		i *= c + 1;
		i += (i << 11);
		i ^= (i >> 7);
	}
	i += (i >> 13);
	i ^= (i << 17);
	i += (i >> 3);
	return i;
}

constexpr unsigned build_hash()
{
	return crude_hash(build_date);
}

#include <iostream>
int main()
{
	std::cout << build_date   << '\n';
	std::cout << build_hash() << '\n';
}
