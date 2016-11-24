#include <memory>

class A {
	virtual void a() {}
};
class B:public A {
	virtual void a() {}
};

std::unique_ptr<B> returnB()
{
	return std::make_unique<B>();
}

int main()
{
	std::unique_ptr<A> decl;
	decl = returnB();
}
