
using u64 = unsigned long long;

u64 lower_mask(unsigned bits) { return (u64(1) << bits) - 1; }
u64 upper_mask(unsigned bits) { return (lower_mask(bits)) << bits; }

u64 clear_top_bit(u64 i) { return (i << 1) >> 1; }

unsigned get_closest_size(u64 value, bool is_signed)
{
	if( value == 0 ) return 1;

	static const u64 powers[] = { 8, 4, 2, 1 };
	static const u64 lookup[] = {
		upper_mask(32),
		upper_mask(16),
		upper_mask(8),
		lower_mask(8)
	};

	for (unsigned i = 0; i < 4; ++i) {
			if (value & (lookup[i] >> is_signed))
		return powers[i];
	}

	return 16;
}


#include <iostream>
int main()
{
	for (int shift = -1; shift < 64; ++shift) {
		u64 val = (shift==-1)?0: u64(1) << shift;
		std::cout << val << ' ' << get_closest_size(val, false)
		                 << ' ' << get_closest_size(val, true) << '\n';
	}
}
