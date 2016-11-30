#include <chrono>
struct stopwatch {
	using clock = std::chrono::steady_clock;
	using notify_func = void(clock::duration);

	clock::time_point start = clock::now();
	notify_func* notify;

	stopwatch(notify_func* notify)
		: notify{ notify }
	{}

	~stopwatch()
	{
		auto end = clock::now();
		notify( end - start );
	}
};

#include <iostream>
void norif( std::chrono::steady_clock::duration dur )
{
	std::cout << dur.count() << '\n';
}

[[gnu::used, gnu::noinline]] void use(int& x) {}
#include <iomanip>
int main()
{
	using namespace std::chrono;
	auto start = system_clock::now();
	auto end   = system_clock::now();
	std::cout << std::setprecision(17) << duration<double>(steady_clock::now().time_since_epoch()).count() << '\n';
	std::cout << (end - start).count() << '\n';
	{
		stopwatch watch{ norif };
		int i;
		use(i);
	}
}
