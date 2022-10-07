#include <vector>
#define _GLIBCXX_DEQUE_BUF_SIZE 131072
#include <deque>
#include <list>
#include <array>
#include <string>
#include <chrono>
#include <iostream>
void reg(long*);
void rir(unsigned long const&) {}
#include <random>
#include <algorithm>
#include <functional>

void use(std::string& s);
int main()
{
    std::vector<std::mt19937::result_type> random_data(std::mt19937::state_size);
    std::random_device source;
    std::generate(std::begin(random_data), std::end(random_data), std::ref(source));
    std::seed_seq seeds(std::begin(random_data), std::end(random_data));
    std::mt19937 eng{seeds};
    std::uniform_int_distribution<> dis(499, 500);
    
	int i = 100000;
	while (i --> 0) {
		auto  j =dis(eng);
		while (j --> 0)
			reg(new long(j));
	}

    std::list<std::array<std::string,1>> vec{100000};

	auto t1 = std::chrono::steady_clock::now();
	for (auto& a : vec)
		for (auto& s : a)
			use(s);
	auto t2 = std::chrono::steady_clock::now();

	std::cout << std::chrono::duration<double>(t2-t1).count() << '\n';
}
