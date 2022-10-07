#include <iostream>
#include <typeinfo>
#include <string>

typedef double f64;

enum class Type {
	Unknown,
	Double,
};

template <typename T>
struct typeof
{
	static const Type value = Type::Unknown;
};

template <>
struct typeof<f64>
{
	static const Type value = Type::Double;
};

template <typename T>
Type getType(T thing)
{
	return typeof<T>::value;
}

int main()
{
	std::cout << (typeid(f64) == typeid(double)) << "\n";
	std::cout << (typeof<f64>::value == typeof<double>::value) << std::endl;
	std::cout << (typeof<f64>::value == getType(std::stod("1.2e+20"))) << std::endl;
}
