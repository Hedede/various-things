
unsigned __int128 fib(decltype(sizeof(int)) n)
{
	return n > 1 ? fib(n - 1) + fib(n - 2) : 1;
}

unsigned __int128 fib_memo(decltype(sizeof(int)) n)
{
	static unsigned __int128 mem[186] {1, 1};
	if (mem[n] == 0) mem[n] = fib(n - 1) + fib(n - 2);
	return mem[n];
}

unsigned __int128 good_fib(decltype(sizeof(int)) n)
{
	constexpr unsigned __int128 N = 0;
	unsigned __int128 fib[2] {1, 1};
	while (n --> 0) {
		fib[N+1] = fib[N+1] + fib[N];
		fib[N]   = fib[N+1] - fib[N];
	}
	return fib[N];
}

unsigned __int128 good_fib2(decltype(sizeof(int)) n)
{
	constexpr unsigned __int128 N = 0;
	unsigned __int128 fib[2] {1, 1};
	while (n --> 0) {
		unsigned __int128 next = fib[N] + fib[N+1];
		fib[N] = fib[N+1];
		fib[N+1] = next;
	}
	return fib[N];
}


unsigned __int128 fib_stack(decltype(sizeof(int)) n)
{
	unsigned __int128 fib[2] {1, 1};
	while (n --> 1)
		fib[n & 1] = fib[0] + fib[1];
	return fib[0];
}



#include <iostream>
int main()
{
	unsigned __int128 sum = 0;
	for (size_t i = 0; i < 10000000; ++i)
		sum += good_fib(185);
	std::cout << (unsigned long)sum << '\n';

	std::cout << (unsigned)fib(44) << '\n';
	std::cout << (unsigned)fib_memo(44) << '\n';
	std::cout << (unsigned)fib_stack(44) << '\n';
	std::cout << (unsigned)good_fib(44) << '\n';
	std::cout << (unsigned)good_fib2(44) << '\n';
	std::cout << (unsigned)fib_memo(44) << '\n';


	std::cout << std::hex << (unsigned long)(good_fib(185) >> 64) << (unsigned long)good_fib(185) << '\n';
	std::cout << std::hex << (unsigned long)(good_fib2(185) >> 64) << (unsigned long)good_fib(185) << '\n';
	std::cout << std::hex << (unsigned long)(fib_stack(185) >> 64) << (unsigned long)good_fib(185) << '\n';
}
