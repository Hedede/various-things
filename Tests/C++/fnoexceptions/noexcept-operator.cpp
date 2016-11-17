#include <type_traits>
#include <stdexcept>
template<typename F>
void func(F&& f) noexcept(noexcept(f()))
{
	f();
}

template<typename F>
struct stru {
	F func;
	~stru() noexcept(false)
	{
		func();
	}
};

template<typename F>
stru<F> str(F&& f)
{
	return {f};
}

void thrower()
{
	throw std::runtime_error{"rozha mne tvoya ne nravitsya"};
}

#include <iostream>
int main()
{
	auto lambda1 = [] () noexcept {};
	auto lambda2 = [] { throw 0; };

	try {
		func(lambda2);
		func(thrower);

		std::cerr << noexcept( func<decltype(lambda1)> ) << '\n';
		std::cerr << noexcept( func<decltype(lambda2)> ) << '\n';
		std::cerr << noexcept( func<decltype(thrower)> ) << '\n';
		std::cerr << "-------" << std::endl;
	} catch(std::runtime_error& err) {
		std::cout << err.what() << '\n';
	} catch(int) {
		std::cout << "catched\n";
	}

	std::cerr << noexcept( func(thrower) ) << '\n';
	std::cerr << noexcept( func(lambda1) ) << '\n';
	std::cerr << noexcept( func(lambda2) ) << '\n';
	std::cerr << "-------" << std::endl;

	try {
		auto st = str(lambda1);
		auto struc = str(thrower);
		std::cerr << noexcept( st.~stru() ) << '\n';
		std::cerr << noexcept( struc.~stru() ) << '\n';
		std::cerr << "-------" << std::endl;
	} catch(std::runtime_error& err) {
		std::cout << err.what() << '\n';
	}

	try {
		auto s = str(lambda2);
		std::cerr << noexcept( s.~stru() ) << '\n';
		std::cerr << "-------" << std::endl;
	} catch(int v) {
		std::cout << "catched" << v << '\n';
	}


}
