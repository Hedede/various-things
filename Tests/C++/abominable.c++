#include <iostream>
struct Test {
	using Func = void();

	Func a;
	Func b;

	using Func2 = void(int x);
	Func2 c;
};

void Test::a() { std::cout << __PRETTY_FUNCTION__ << '\n'; }
void Test::b() { std::cout << __PRETTY_FUNCTION__ << '\n'; }
void Test::c(int x) { std::cout << __PRETTY_FUNCTION__ << ' ' << x << '\n'; }

using Gunc = void(Test&);
Gunc f;
void f(Test& t) { t.a(); }

int main()
{
	Test t;
	t.a();
	t.b();
	t.c(1);
	f(t);
}
