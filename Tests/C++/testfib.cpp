#include <iostream>

long calls = 0;
long leafs = 0;
void f1(int n) {
	if (n <= 1)
		++leafs;
	else ++calls, f1(n-1), f1(n-2);
}
long f(int n) {
	if (n <= 1)
		return n;
	else return f(n - 1) + f(n - 2) + 1;
}


#include <chrono>
int main(int, char**argv)
{
	int n = std::stoi(argv[1]);
	f1(n);
	auto beg = std::chrono::steady_clock::now();
	auto res = f(n);
	auto end = std::chrono::steady_clock::now();
	auto dur = std::chrono::duration<double>(end-beg);
	std::cout << calls << ' ' << leafs << ' ' << res << ' ' << dur.count() << " s\n";
}

