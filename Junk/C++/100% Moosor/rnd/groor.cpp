#include <vector>
#include <string>
#include <chrono>
#include <iostream>
void reg(long*);
void rir(unsigned long const&) {}

void use(std::string& s);
int main()
{
    std::vector<std::string> vec{100000};

	auto t1 = std::chrono::steady_clock::now();
	for (auto& s : vec)
		use(s);
	auto t2 = std::chrono::steady_clock::now();

	std::cout << std::chrono::duration<double>(t2-t1).count() << '\n';
}
