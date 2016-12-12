#include <iostream>
#include <iostream>
#include <functional>
#include <type_traits>
#include <cmath>
#include <limits>
#include <iomanip>

//! Divide two integers, rounding result to nearest value
//! Divide two integers, rounding result to nearest value
template<typename T>
constexpr auto div_round(T v, T d)
{
	return (v > 0) ?
	     (v + (d/2)) / d :
	     (v - (d/2)) / d;
}

template<typename T, template<typename> typename P>
struct require_t : std::enable_if<P<T>::value, T> { };

template<typename T, template<typename> typename P>
using require = require_t<T, P>;

template<typename T>
constexpr auto restr(T x, T y) -> require<T, std::is_integral>
{
	return x - div_floor(x, y) * y;
}

template <typename T, typename U>
auto iremainder(T value, U period) -> auto
{
	return value - div_round( value, period ) * period;
}

template <typename T, typename U>
auto fremainder(T value, U period) -> auto
{
	return value - std::round( value / period) * period;
	// TODO: edge cases?
	auto adjusted = value + period / 2;
	return value - std::floor( adjusted / period ) * period;
}
int main()
{
	for(int i = -720; i < 721; ++i)
	{
		std::cerr << i;
		std::cerr << ' ' << iremainder(i, 360);
		std::cerr << ' ' << fremainder(double(i), 360.0) << '\n';
	}
}

#if 0
template <typename T, typename U>
auto rmainder(T value, U period)
{
	auto adjusted = value;
	if (value < 0)
		adjusted -= period / 2 - 1;
	else
		adjusted += period / 2;
	return value - ( adjusted / period ) * period;
}

template <typename T, typename U>
auto frmainder(T value, U period)
{
	auto adjusted = value + period / 2;
	return value - std::floor( adjusted / period ) * period;
}

template<typename T>
T Mod(T x, T y)
{
	auto adjusted = x;

	adjusted += y / 2;

	double m = x - y * floor(adjusted/y);

	// handle boundary cases resulted from floating-point cut off:

	if (y > 0)              // modulo range: [0..y)
	{
		if (m>=y)
		{           // Mod(-1e-16             , 360.    ): m= 360.
			std::cout << "DOOT DOOT\n";
			return 0;
		}

		if (m<0 )
		{
			if (y+m == y) {
				std::cout << "DOOT DOOT\n";
				return 0  ; // just in case...
			}
		}
	}

	return m;
}

#include <cstring>
template<typename Output, typename Input>
Output reinterpret(Input&& in) {
	Output out;
	std::memcpy(&out, &in, sizeof(Input));
	return out;
}
int main()
{
	/*
	for(int i = -720; i < 721; ++i)
	{
		std::cerr << i;
		std::cerr << ' ' << rmainder(i, 360);
		std::cerr << ' ' << frmainder(double(i), 360.0);
		std::cerr << ' ' << remainder(double(i), 360.0);
		std::cerr << ' ' << Mod(double(i) + 180.0, 360.0) - 180.0 << '\n';
	}*/

	double start = -2*M_PI-0.0001;
	long li = -reinterpret<long>( start );
	long lj = reinterpret<long>(  start );
	std::cout << li << ' ' << lj << std::endl;
	for (double i = start; i < std::nexttoward((2*M_PI), (2*M_PI+1)); i = std::nexttoward(i,(2*M_PI+1)) )
	//for (double i = -2*M_PI; i <2*M_PI+0.01; i += 0.01 )
	{
		/*
		std::cerr << i;
		std::cerr << std::fixed;
		std::cerr << ' ' << frmainder(double(i), 2*M_PI);
		std::cerr << ' ' << Mod(double(i) + M_PI, 2*M_PI) - M_PI << '\n';
		*/
		std::cout.precision(17);
		std::cout << std::fixed;
		//double a = frmainder(double(i), 2*M_PI);
		//double b = Mod(double(i), 2*M_PI);
		//if (a != b) std::cout << "okay, it is actually needed: " << a << ' ' << b << ' ' << a - b << "\n";

		Mod(i, 2*M_PI);

		if (li++ % (2<<24) == 0)
			std::cout << i << ' ' << li << std::endl;
	}
/*
	std::cerr << -2*M_PI << ' ' << remainder(-2*M_PI, 2*M_PI) << '\n';
	std::cerr << -M_PI-0.01 << ' ' << remainder(-M_PI-0.01, 2*M_PI) << '\n';
	std::cerr << -M_PI << ' ' << remainder(-M_PI, 2*M_PI) << '\n';
	if (remainder(-M_PI, 2*M_PI) == -M_PI)
		std::cerr << "WROOONG" << '\n';
	std::cerr << M_PI << ' ' << remainder(M_PI, 2*M_PI) << '\n';
	std::cerr << M_PI+0.01 << ' ' << remainder(M_PI+0.01, 2*M_PI) << '\n';
	std::cerr << 2*M_PI << ' ' << remainder(2*M_PI, 2*M_PI) << '\n';*/
}
#endif
