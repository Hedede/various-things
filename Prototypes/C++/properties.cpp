#include<utility>
template<typename Get, typename Set>
class property {
	Get get;
	Set set;
public:
	property(Get&& get, Set&& set) : get(get), set(set) {}

	template<typename U>
	auto&& operator=(U&& u)
	{
		return set(std::forward<U>(u));
	}

	template<typename U>
	operator U&()
	{
		return get();
	}
};

#include <iostream>
class myclass {
	int _x, _y;
#if __cplusplus < 201601L
	struct Setter {
		int& x;
		int& operator()(int y)
		{
			std::cout << "set _x to " << y << '\n';
			return x = y;
		}
	};

	struct Getter {
		int& x;
		int& operator()()
		{
			std::cout << "get _x, x == " << x << '\n';
			return x;
		}
	};
public:
	property<Getter, Setter> x {
		Getter{_x},
		Setter{_x}
		/* v C++17 replace with: */
		/* [&] { return _x; }, */
		/* [&] (int y) { return _x = y; } */
	};
#else
public:
	property x {
		[&] { return _x; },
		[&] (int y) { return _x = y; }
	};
#endif
};

int main()
{
	myclass c;
	c.x = 10;
	int y = c.x;
}
