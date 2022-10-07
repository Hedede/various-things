#include <iostream>
#include <algorithm>
#include <set>

bool loss(std::string a, std::string b)
{
	std::sort(std::begin(a), std::end(a));
	std::sort(std::begin(b), std::end(b));

	return a < b;
}

size_t dice(std::string s)
{
	return 0;
}

size_t val(char c)
{
	return c - '1' + 1;
}


struct sorte {
	std::string str;
	bool operator <(sorte const& o)const
	{
		return loss(str, o.str);
		return dice(str) < dice(o.str);
	}
};

size_t sum(sorte sorrte)
{

		size_t a{},b{};
		for (char c : sorrte.str)
			a += val(c);

		return a;
}

int main()
{
	std::multiset<sorte> set;

	std::string sing;
	while (std::cin >> sing) {
		sorte s{sing};
		set.insert(s);
	}

	std::set<sorte> keys;

	for (auto it = std::begin(set); it != std::end(set); it = set.upper_bound(*it))
		keys.insert(*it);

	sorte* zo = 0;

	std::multiset<size_t> nums;

	for (auto koy : keys) {
		std::cout << "(" << koy.str << "," << set.count(koy) << ")";
	}

	for (auto koy : keys) {
		std::cout << "(" << koy.str << "," << set.count(koy) << ")";
	}
	std::cout <<"\n";

	for (auto koy : keys) {
		nums.insert(set.count(koy));
		size_t cunt = set.count(koy);
		std::cout << "{" << koy.str;
		while (--cunt > 0)
			std::cout << "," << koy.str;
		std::cout << "}" << set.count(koy) << "\n";
	}
	std::cout << keys.size() << "\n\n";

	/*
	for (auto num : nums)
		std::cout << num << " " << nums.count(num) << "\n";


	std::cout << keys.size() << "\n";

	for (auto& thing : set) {
		auto koy = keys.find(thing);
		if (!zo) zo = new sorte{*koy};
		if (koy->str != zo->str) std::cout << "\n";
		if (koy->str != zo->str) *zo = *koy;

		std::cout << thing.str << " " << koy->str << " " << set.count(thing) << " " << sum(thing) << "\n";
	}*/
}
