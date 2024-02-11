#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>

int main()
{
	constexpr size_t size = 5ull*1024ull*1024*1024;
	auto* yuy = new unsigned char[size];
	auto* end = yuy+size;
	std::generate(yuy, end, [n = 0u] () mutable { return ++n % 256; });

	std::cout << "done\n";

	using namespace std::chrono_literals;
	std::this_thread::sleep_for(10s);
}
