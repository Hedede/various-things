struct closs {
	static const char classname[];
	virtual char const* className() const
	{
		return classname;
	}

};
const char closs::classname[] = "class";

#include <iostream>
int main()
{
	closs cla;
	closs clo;

	std::cout << (cla.className() == clo.className()) << "\n";
	std::cout << (sizeof(closs::classname)) << "\n";
}
