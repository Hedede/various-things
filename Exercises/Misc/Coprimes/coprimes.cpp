#include <stdio.h>
#include <stdint.h>
#include <algorithm>

typedef uint64_t u64;

long fibs[40];


void gen_sequence()
{
	u64 fib1 = 1;
	u64 fib2 = 1;
	u64 fib = 0;

	fibs[0]=fib1;
	fibs[1]=fib2;
	fprintf(stderr,"[0]=%ld\n",fib1);
	fprintf(stderr,"[1]=%ld\n",fib2);

	for (int i=2; i < 40; ++i) {
		fib = fib1 + fib2;
		fib1 = fib2;
		fib2 = fib;

		fibs[i]=fib;
		fprintf(stderr,"[%d]=%ld\n",i,fib);
	}
}

u64 gcd(u64 m, u64 n)
{
	return n ? gcd(n, m % n) : m;
}

void print_sorted(u64 a, u64 b, u64 c)
{
	if (a > c)
		std::swap(a, c);

	if (a > b)
		std::swap(a, b);

	if (b > c)
		std::swap(b, c);

	printf("%lu|%lu|%lu\n",a,b,c);
}

void list_coprimes(u64 i, u64 j, u64 max)
{
	if (gcd(fibs[i], fibs[j]) > 1)
		return;

	for(u64 k = j + 1; k < max; ++k) {
		if (gcd(fibs[i], fibs[k]) == 1 && gcd(fibs[j], fibs[k]) == 1)
			print_sorted(i, j, k);
	}
}

void coprimes(u64 max)
{
	for (u64 i = 0; i < max; ++i) {
		for (u64 j = i + 1; j < max; ++j) {
			list_coprimes(i, j, max);
		}
	}
}


int main() {
	gen_sequence();
	coprimes(40);
}
