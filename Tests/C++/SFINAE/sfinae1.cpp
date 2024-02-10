#include <iostream>

//#define CORRECT
//#define AMBIGUOUS
//#define CANTDEDUCE

#if defined(CORRECT)
struct Test {
	using type = int;
};

template <typename T> 
void f(typename T::type)
{
	std::cout <<"A\n";
}

template <typename T> 
void f(T)
{
	std::cout <<"B\n";
}

int main() {
	f<Test>(10);
	f<int>(10);
}

#elif defined(AMBIGUOUS)
template<typename F>
auto call_functor(F func) -> typename F::type
{
	std::cout <<"A\n";
	return func();
}

template<typename F>
void call_functor(F func)
{
	std::cout <<"B\n";
	func();
}

int main()
{
	call_functor( [] { std::cout << "Hello"; } );

	struct functor {
		using type = std::string;
		type operator()()
		{
			return "B";
		}
	};

	auto x = call_functor( functor{} );
	std::cout << x;
}
#elif CANTDEDUCE
#include <type_traits>
template<typename F>
void test(std::enable_if_t<std::is_integral<F>::value, F> arg)
{
	std::cout << arg <<"A\n";
}

template<typename F>
void test(std::enable_if_t<std::is_floating_point<F>::value, F> arg)
{
	std::cout << arg <<"B\n";
}

int main()
{
	//test(10);
	//test(10.0);

	test<float>(10);
	test<int>(10.0);
}
#else
#include <type_traits>
template<typename F, typename = std::enable_if_t<std::is_integral<F>::value>>
void test(F arg)
{
	std::cout <<"A ";
}

template<typename F, typename = std::enable_if_t<std::is_floating_point<F>::value>>
void test(F arg)
{
	std::cout <<"B ";
}

int main()
{
	test(10);
	test(10.0);
}
#endif
