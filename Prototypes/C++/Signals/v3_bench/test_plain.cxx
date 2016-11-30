#include "header.h"

double test_plain()
{
	auto sta = steady_clock::now();
	val = 0;
	for (unsigned i = 0; i < num_iter; ++i)
		++val;
	auto end = steady_clock::now();
	return double(nanoseconds(end - sta).count()) / num_iter;
}
