#include <iostream>
#include <string>
#include <aw/types/support/reinterpret.h>
int main()
{
    int i = 0;
    double p = 0.001;
    double t = 0;
    while (i < 10000000000)
    {
	    auto a = ++i*p;
	    auto b = (t+=p);
	    auto aa = aw::reinterpret<uintptr_t>(a);
	    auto bb = aw::reinterpret<uintptr_t>(b);
	    std::cout << a << ' ' << b << '\n';
	    //std::cout << ((aa>bb)?(aa-bb):(bb-aa)) << '\n';
    }
    
}

