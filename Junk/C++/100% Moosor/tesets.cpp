

#include <iostream>
class Class {
	friend int main();
void arc(Class* closs)
{
	Class* cyc = this->cyc;
	if (closs == cyc)
		return;
	std::cout << "nulling";
	cyc = nullptr;
}

Class* cyc;
};
int main()
{
	Class cc;
	cc.cyc = &cc;
	Class ccc;
	cc.arc(&ccc);
	std::cout << cc.cyc;
	return 0;
}
