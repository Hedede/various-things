#include <iostream>
#include <algorithm>
#include <vector>
int main()
{
	std::cout << __cplusplus <<'\n';
	std::vector<int> vec{1,2,3};
	auto b = vec.begin();
	std::rotate(b + 2, b+ 0 , b+0+1);
	for (int i:vec)
		std::cout <<i<<' ';
	std::cout <<'\n';
}
