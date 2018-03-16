#include <cstdint>
#include <random>

class Random {
	std::default_random_engine engine;
	// std::minstd_rand engine;
	// std::mt19937 engine;

	std::uniform_real_distribution<double> uniform_dist;
	std::normal_distribution<double> normal_dist;
	// std::uniform_int_distribution<uint64_t> uniform_dist;
public:
	Random(double min, double max)
		: engine(std::random_device()()),
		  uniform_dist(min, max)
	{
	}

	Random(double min, double max, uint64_t seed)
		: engine(seed),
		uniform_dist(min, max)
	{
	}

	double get()
	{
		return uniform_dist(engine);
	}
};

#include <iostream>
int main()
{
	double min_value = 50;
	double max_value = 100;
	Random rnd(min_value, max_value);

	for (int n = 0; n < 50; ++n)
		std::cout << rnd.get() << ' ';

	std::cout << std::endl;
}
