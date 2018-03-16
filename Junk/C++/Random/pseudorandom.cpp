#include <map>
#include <string>
#include <iostream>

#include <random>

int main()
{
	// seed
	std::random_device device;

	std::mt19937 engine1(device());
	std::default_random_engine engine2(device());

	int mean = 50;
	// равномерное распределение
	std::uniform_int_distribution<int> uniform_dist(mean - 10, mean + 10);
	// гауссово (нормальное) распределение
	std::normal_distribution<> normal_dist(mean, 10);

	struct {
		std::map<int, int> e1_normal;
		std::map<int, int> e1_uniform;
		std::map<int, int> e2_normal;
		std::map<int, int> e2_uniform;
	} occurences;

	for (int n = 0; n < 10000; ++n) {
		int random_number = normal_dist(engine1);
		occurences.e1_normal[random_number] += 1;

		random_number = uniform_dist(engine1);
		occurences.e1_uniform[random_number] += 1;

		random_number = normal_dist(engine2);
		occurences.e2_normal[random_number] += 1;

		random_number = uniform_dist(engine2);
		occurences.e2_uniform[random_number] += 1;
	}

	auto o1 = std::begin(occurences.e1_normal);
	auto o3 = std::begin(occurences.e2_normal);
	auto end = std::end(occurences.e1_normal);

	while (o1 != end) {
		std::cout << o1->first << ": " << o1->second << "\t"
		          << o3->first << ": " << o3->second << "\n";
		++o1;
		++o3;
	}

	auto o2 = std::begin(occurences.e1_uniform);
	auto o4 = std::begin(occurences.e2_uniform);
	end = std::end(occurences.e1_uniform);
	while (o2 != end) {
		std::cout << o2->first << ": " << o2->second << "\t"
		          << o4->first << ": " << o4->second << "\n";
		++o2;
		++o4;
	}
}
