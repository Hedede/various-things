#include "header.h"

double test_vfun(bool arg)
{
	auto sta = steady_clock::now();
	counter* ctr = arg ? (counter*)new counter_dec : (counter*)new counter_inc;
	v.ctr = ctr;
	for (unsigned i = 0; i < num_iter; ++i)
		v.invoke();
	auto end = steady_clock::now();
	return double(nanoseconds(end - sta).count()) / num_iter;
}
