#include <cassert>
#include <cstring>
#include <iostream>
#include <string>
#include <cstdint>
#include <limits>
char symbol[] = {
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
	'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
	'U', 'V', 'W', 'X', 'Y', 'Z'
};

//! Compute log2 of an integer value
constexpr size_t log2(uint64_t value)
{
	if (value == 0) return -1;
	size_t ret = 0;
	while (value >>= 1)
		++ ret;
	return ret;
}

//! Check if value is a power of 2
constexpr bool isPowerOf2(uint64_t value)
{
	return value && !(value & (value - 1));
}

template <size_t base, typename Int>
std::string int_to_str_generic(Int i)
{
	assert(1 < base && base < 37 && "invalid base");

	constexpr size_t log2 = ::log2(base);
	constexpr size_t width = std::numeric_limits<Int>::digits;

	char tmpbuf[width/log2+1];

	bool neg = i < 0;
	uint64_t q = neg ? -i : i;

	char* tmp = tmpbuf + sizeof(tmpbuf);

	// This is pointless, optimized code generated for both branches
	// is same for power-of-2 base
	if (isPowerOf2(base)) {
		while (q >= base) {
			*(tmp--) = symbol[q & (base - 1)];
			q >>= log2;
		}
	} else {
		while (q >= base) {
			*(tmp--) = symbol[q % base];
			q /= base;
		}
	}

	*(tmp--) = symbol[q];
	if (neg)
		*(tmp--) = '-';

	return std::string(tmp+1, tmpbuf + sizeof(tmpbuf) + 1);
}

int main(int, char** a)
{
	uint64_t z = std::strtoull(a[1], 0, 10);
	std::cout << z <<std::endl;
	std::cout << int_to_str_generic< 2 >(z) << std::endl;
	std::cout << int_to_str_generic< 3 >(z) << std::endl;
	std::cout << int_to_str_generic< 4 >(z) << std::endl;
	std::cout << int_to_str_generic< 5 >(z) << std::endl;
	std::cout << int_to_str_generic< 6 >(z) << std::endl;
	std::cout << int_to_str_generic< 7 >(z) << std::endl;
	std::cout << int_to_str_generic< 8 >(z) << std::endl;
	std::cout << int_to_str_generic< 9 >(z) << std::endl;
	std::cout << int_to_str_generic< 10>(z) << std::endl;
	std::cout << int_to_str_generic< 11>(z) << std::endl;
	std::cout << int_to_str_generic< 12>(z) << std::endl;
	std::cout << int_to_str_generic< 13>(z) << std::endl;
	std::cout << int_to_str_generic< 14>(z) << std::endl;
	std::cout << int_to_str_generic< 15>(z) << std::endl;
	std::cout << int_to_str_generic< 16>(z) << std::endl;
	std::cout << int_to_str_generic< 17>(z) << std::endl;
	std::cout << int_to_str_generic< 18>(z) << std::endl;
	std::cout << int_to_str_generic< 19>(z) << std::endl;
	std::cout << int_to_str_generic< 20>(z) << std::endl;
	std::cout << int_to_str_generic< 21>(z) << std::endl;
	std::cout << int_to_str_generic< 22>(z) << std::endl;
	std::cout << int_to_str_generic< 23>(z) << std::endl;
	std::cout << int_to_str_generic< 24>(z) << std::endl;
	std::cout << int_to_str_generic< 25>(z) << std::endl;
	std::cout << int_to_str_generic< 26>(z) << std::endl;
	std::cout << int_to_str_generic< 27>(z) << std::endl;
	std::cout << int_to_str_generic< 28>(z) << std::endl;
	std::cout << int_to_str_generic< 29>(z) << std::endl;
	std::cout << int_to_str_generic< 30>(z) << std::endl;
	std::cout << int_to_str_generic< 31>(z) << std::endl;
	std::cout << int_to_str_generic< 32>(z) << std::endl;
	std::cout << int_to_str_generic< 33>(z) << std::endl;
	std::cout << int_to_str_generic< 34>(z) << std::endl;
	std::cout << int_to_str_generic< 35>(z) << std::endl;
	std::cout << int_to_str_generic< 36>(z) << std::endl;
}

