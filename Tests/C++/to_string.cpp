#include <string>

template<typename T>
struct string_converter;

template <typename T>
std::string to_string(T const& value)
{
	return string_converter<T>{ }(value);
}

template<>
struct string_converter<int> {
	std::string operator()(int v)
	{
		return std::to_string(v);
	}
};

#include <iostream>
int main()
{
	std::cout << to_string(100) << '\n';
}
