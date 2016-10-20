

void def_func(char const* fun, int ret, int arg...);





int main()
{
	def_func("func", 0, 2, 3, 1, 2, 3, 0);
	def_func("func", 0, 1, 2, 3, 0);
	def_func("func", 0, 0);
}






#include <iostream>
std::string name(int type)
{
	switch (type) {
	case 0:
		return "void";
	case 1:
		return "int";
	case 2:
		return "float";
	case 3:
		return "string";
	}

	return "";
}
void def_func(char const* fun, int ret, int arg...)
{
	std::cout << name(ret) << " " << fun << "(";
	if (arg)
		std::cout << name(arg);

	int* ptr = &arg;
	while (*--ptr)
		std::cout << ", " << name(*ptr);

	std::cout << ")\n";
}
