struct _1 {};
struct _2 {};

template<typename U, typename T>
struct replace {
	using type = U;
};

template<typename T>
struct replace<_1, T> {
	using type = T;
};

template<typename U, typename T>
using replace_1 = typename replace<U, T>::type;

template<typename F, typename T>
constexpr bool test = expand<F,T>::value;


#include <iostream>
#include <type_traits>

template<typename T>
void debug() {
	std::cout << __PRETTY_FUNCTION__ << '\n';
}

int main()
{
	std::cout << std::boolalpha;
	std::cout << test<std::is_same<_1, int>, int> << '\n';

	debug<expand<std::is_same<_1, int>, int>::type>();
}
//static_assert(  == true );
