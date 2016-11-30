#include "header.h"
void st_counter::count(countable& ctr) const { ++ctr; }
void fun_counter::invoke() { ctr(*this); }
