#include <random>

class Random {
	std::default_random_engine engine;
	std::uniform_int_distribution<int> uniform_dist;
public:
	Random(int min, int max)
		: engine(std::random_device()()),
		  uniform_dist(min, max)
	{
	}

	Random(int min, int max, int seed)
		: engine(seed),
		uniform_dist(min, max)
	{
	}

	int get()
	{
		return uniform_dist(engine);
	}
};

#include <iostream>
int main()
{
	int min_value = 50;
	int max_value = 100;
	Random rnd(min_value, max_value);

	for (int n = 0; n < 50; ++n)
		std::cout << rnd.get() << ' ';

	std::cout << std::endl;
}
