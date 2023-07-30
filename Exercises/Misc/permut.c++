#include <algorithm>
#include <set>
#include <iostream>

int main(int c, char** v)
{
	std::string s = "MARGARITA";

	if (c>1)
		s = v[1];

	std::set<std::string> set;
	std::sort(begin(s),end(s));
	do {
		set.insert(s);
	} while (std::next_permutation(begin(s),end(s)));

	std::cout << set.size() << '\n';
}
