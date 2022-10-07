#include <vector>
#include <iostream>
#include <algorithm>

int main()
{
	std::vector<int> v{1,2,3,4,5};

	auto ne = std::remove_if(v.begin(),v.end(),[] (int i) { return i < 3; });

	v.resize(ne - v.begin());

	for (auto i : v)
		std::cout << i << '\n';
}
