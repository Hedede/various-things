
#include "header.h"


vf_counter v;
fun_counter f;
delectr d;

volatile int val;


void delectr::invoke()
{
	coll(collubl, *this);
}

#include <iostream>
int main(int, char** arg)
{
	std::cout << "vfu: " << test_vfun(false) << '\n';
	std::cout << "fun: " << test_fun(false) << '\n';
	std::cout << "tpl: " << test_tpl(false) << '\n';
	std::cout << "\n";
	std::cout << "vfu: " << test_vfun(true) << '\n';
	std::cout << "fun: " << test_fun(true) << '\n';
	std::cout << "tpl: " << test_tpl(true) << '\n';
	std::cout << "\n";
	std::cout << "pla: " << test_plain() << '\n';



}
