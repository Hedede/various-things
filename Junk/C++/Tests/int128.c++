

#include <iostream>

//using uint64_t = unsigned long long;

#ifdef __i386
inline uint64_t rdtsc()
{
	uint64_t x;
	asm volatile ("rdtsc" : "=A" (x));
	return x;
}
#elif __amd64
inline uint64_t rdtsc()
{
	uint64_t a, d;
	asm volatile ("rdtsc" : "=a" (a), "=d" (d));
	return (d<<32) | a;
}
#endif

void use(__int128&);

int main(int argv, char** argc)
{
	__int128 volatile i1 = (__int128(6182712187) << 78) + (__int128(128371231273));
	__int128 volatile i2 = (__int128(213812931) << 78) + (__int128(1213912));
	uint64_t time = rdtsc();
	__int128 volatile i3 = i1 / i2;
	uint64_t time2 = rdtsc();
	std::cout << time2 - time << '\n';
	std::cout << std::hex << uint64_t(i3 >> 64) << uint64_t(i3) << '\n';

	uint64_t volatile x1 = (uint64_t(6182712187) << 22) + ((128371231273));
	uint64_t volatile x2 = (uint64_t(213812931) << 22) + ((1213912));
	time = rdtsc();
	uint64_t volatile x3 = x1 / x2;
	time2 = rdtsc();
	std::cout << time2 - time << '\n';
}
