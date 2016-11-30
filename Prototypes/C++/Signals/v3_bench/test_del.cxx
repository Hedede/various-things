#include "header.h"
double test_tpl(bool arg)
{
	auto sta = steady_clock::now();
	st_counter sct;
	counter* vct = new counter_inc;

	if (!arg) {
		d.collubl = (void*)&sct;
		d.coll = delegate<st_counter const, void(countable&)>::call<&st_counter::count>;
	} else {
		d.collubl = (void*)vct;
		d.coll = delegate<counter, void(countable&)>::call<&counter::count>;
	}

	for (unsigned i = 0; i < num_iter; ++i)
		d.invoke();
	auto end = steady_clock::now();
	return double(nanoseconds(end - sta).count()) / num_iter;
}
