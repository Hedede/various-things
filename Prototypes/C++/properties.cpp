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

	using get_return_type = decltype(get())&;
	operator get_return_type()
	{
		return get();
	}

	using get_const_return_type = decltype(get())const&;
	operator get_const_return_type() const
	{
		return get();
	}
};

#include <iostream>
class myclass {
	int _x, _y;
#if __cplusplus < 201701L
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
		int& operator()() const
		{
			std::cout << "get _x, x == " << x << '\n';
			return x;
		}
	};
public:
	property<Getter, Setter> x {
		Getter{_x},
		Setter{_x}
	};
#else
// requires template class argument deduction
public:
	property x {
		[&] {
			std::cout << "get _x (" << _x << ")\n";
			return _x;
		},
		[&] (int y) {
			std::cout << "set _x = " << y << '\n';
			return _x = y;
		}
	};
#endif
public:
	myclass(int x, int y) : _x(x), _y(y) {}
};

int main()
{
	myclass c { 1, 2 };
	c.x = 10;
	int y = c.x;

	myclass const cc { 1, 2 };
	y = cc.x;
}
