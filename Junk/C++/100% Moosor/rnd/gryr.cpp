#include <vector>
#include <random>
#include <algorithm>
#include <string>
#include <chrono>
#include <iostream>
#include <functional>
void reg(long*);
void rir(unsigned long const&) {}
void use(std::string& s);
std::string vat[100000];
int main()
{
    std::vector<std::mt19937::result_type> random_data(std::mt19937::state_size);
    std::random_device source;
    std::generate(std::begin(random_data), std::end(random_data), std::ref(source));
    std::seed_seq seeds(std::begin(random_data), std::end(random_data));
    std::mt19937 eng{seeds};

    std::uniform_int_distribution<> dis(10, 20);

    std::vector<std::string*> vec;
	int i = 100000;
	while (i --> 0) {
		auto  j =dis(eng);
		while (j --> 0)
			reg(new long(j));
		vec.push_back(vat+i);
		use(*vec.back());
	}

	auto t1 = std::chrono::steady_clock::now();
	for (auto s : vec)
		use(*s);
	auto t2 = std::chrono::steady_clock::now();

	std::cout << std::chrono::duration<double>(t2-t1).count() << '\n';
}
