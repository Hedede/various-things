#include <cstddef>
#include <iostream>

constexpr uint64_t base = (1ul << 32);
struct fixed {


	uint64_t val;
};

	std::ostream& operator<<(std::ostream& os, fixed const& val)
	{
		std::cout << double(val.val) / base;
		return os;
	}
int main()
{
	uint64_t a = 61 * base + 1073741824;
	uint64_t b = 15 * base;
	
	std::cout << a <<"\n";
	std::cout << b <<"\n";
	std::cout << base <<"\n";

	uint64_t ad = a / b;
	uint64_t am = a % b;
	uint64_t bd = base / b;
	uint64_t bm = base % b;

	std::cout << std::string(6,'-') << 1 <<  std::string(6,'-') <<"\n";
	std::cout << ad <<"\n";
	std::cout << am <<"\n";
	std::cout << bd <<"\n";
	std::cout << bm <<"\n";
	
	uint64_t p1 = ad * bd;
	uint64_t p2 = ad * bm;
	uint64_t p3 = am * bd;
	uint64_t p4 = am * bm;

	std::cout << std::string(6,'-') << 2 <<  std::string(6,'-') <<"\n";
	std::cout << p1 <<"\n";
	std::cout << p2 <<"\n";
	std::cout << p3 <<"\n";
	std::cout << p4 <<"\n";

	uint64_t v1 = p1 * b;
	uint64_t v2 = p2 + p3;
	uint64_t v3 = p4 / b;

	std::cout << std::string(6,'-') << 3 <<  std::string(6,'-') <<"\n";
	std::cout << v1 <<"\n";
	std::cout << v2 <<"\n";
	std::cout << v3 <<"\n";

	std::cout << std::string(6,'-') << 4 <<  std::string(6,'-') <<"\n";
	std::cout << v1 + v2 + v3 << "\n";

	std::cout << std::string(6,'-') << 5 <<  std::string(6,'-') <<"\n";
	std::cout << fixed{v1 + v2 + v3} << "\n";

	using uint128_t = unsigned __int128;

	uint128_t a8 = a;
	a8 *= base;
	a8 /= b;
	std::cout << double(uint64_t(a8)) / base << "\n";
}
