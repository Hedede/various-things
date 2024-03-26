#include <vector>
#include <algorithm>
#include <numeric>
#include <iostream>

template<typename Iter>
Iter sieve(Iter b, Iter e, int upto)
{
	int c = *b;
	while (c*c < upto) {
		e = std::remove_if(++b, e, [c] (int i) { return !(i % c); });
		c = *b;
	}
	return e;
}

void sieve(int x)
{
	std::vector<int> v (x-1);
	std::iota(begin(v), end(v), 2);
	auto e = sieve(begin(v), end(v), x);
	v.erase(e, end(v));
	for (int n : v) std::cout << n << ' ';
	std::cout << "\b\n";
}

int main()
{
	sieve(2);
	sieve(3);
	sieve(10);
	sieve(100);
}
