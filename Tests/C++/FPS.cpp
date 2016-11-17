void __attribute__((used)) use(float const& f) {}
void __attribute__((used)) use(int const& f) {}

#include <chrono>
#include <string>
#include <iostream>
int main()
{
	std::string s = "3.14124125";

	auto start = std::chrono::high_resolution_clock::now();
	auto num = 4000000u;
	for (auto i = 0ul; i < num; ++i) {
		use(std::stoi(s));
	}
	auto end = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> duration = end - start;
	std::cout << duration.count() << std::endl;
}
