#include <iostream>
template<typename T>
void vunc(void* t)
{
	std::cout << *reinterpret_cast<T*>(t) << "\n";
}

template<typename... Ts>
auto stuff(unsigned i)
{
	using func = void(*)(void*);
	static func table [sizeof...(Ts)] {
		(vunc<Ts>)...
	};

	return table[i];
}

int main()
{
	auto func = stuff<int,long>(1);
	func((void*)main);

	std::string s;
	int* mem;

	new (mem) std::string(s);
}

