#include "header.h"
double test_fun(bool arg)
{
	auto sta = steady_clock::now();
	st_counter sct;
	counter* vct = new counter_inc;

	if (!arg)
		f.ctr = [sct] (countable& ctr) { return sct.count(ctr); };
	else
		f.ctr = [vct] (countable& ctr) { return vct->count(ctr); };

	for (unsigned i = 0; i < num_iter; ++i)
		f.invoke();
	auto end = steady_clock::now();
	return double(nanoseconds(end - sta).count()) / num_iter;
}
