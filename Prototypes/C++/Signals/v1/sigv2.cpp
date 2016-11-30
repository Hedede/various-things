#include <thread>
#include "signal.h"

using namespace aw::signals;

#include <iostream>
struct test1 : observer {
	void on_signal()
	{
		std::cout << "blyad, I replaced somebodys 7500 lines of code with 50\n";
	}
};

struct test2 : observer {
	void on_signal()
	{
		std::cout << "just testin'\n";
	}
};

struct test3 : observer {
	void panic()
	{
		std::cout << "PANIC!'\n";
	}
};

template<typename T>
void pretty()
{
	std::cout<< __PRETTY_FUNCTION__ << "\n";
}

int main()
{
	using namespace std::chrono_literals;

	signal<void()> sig;
	{
		test3 slo;
		sig.connect(slo, &test3::panic);
		sig();
	}

	sig();

	test3 slo;
	{
		signal<void()> sig;
		sig.connect(slo, &test3::panic);
		sig();
	}

	sig();

	sig.connect(slo, &test3::panic);

	sig();
}
