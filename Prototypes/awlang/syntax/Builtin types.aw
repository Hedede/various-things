// Builtin types:
uint   — unsigned integer
	uint is largest uint that fits into CPU register
	uint<N> is uint of concrete size
int    — signed integer
	same
float  — floating point number
	same

bool   — uint<1>

// pseudocode for builtin uint
// (pseudocode as in "how it looks to the compiler, but not defined in source files")
template <uint Bits = uint::max_bits>
class uint {
	constexpr uint bits = Bits;

	operator ++ () -> uint&;
	operator -- () -> uint&;
	
	operator ~ () const -> uint;
	operator ! () const -> uint;
	operator - () const -> uint;
	operator + () const -> uint;

	operator += (uint) -> uint&;
	operator -= (uint) -> uint&;
	operator *= (uint) -> uint&;
	operator /= (uint) -> uint&;
	operator %= (uint) -> uint&;

	operator |= (uint) -> uint&;
	operator &= (uint) -> uint&;
	operator ^= (uint) -> uint&;

	operator <<= (uint) -> uint&;
	operator >>= (uint) -> uint&;

	operator == (uint) -> bool;
	operator != (uint) -> bool;
	operator >= (uint) -> bool;
	operator <= (uint) -> bool;
	operator >  (uint) -> bool;
	operator <  (uint) -> bool;
}

class uint {
	constexpr uint max_bits = /* implementation-defined */;
};

operator + (uint, uint) -> uint { return x += y; }
operator - (uint, uint) -> uint { return x -= y; }
operator * (uint, uint) -> uint { return x *= y; }
operator / (uint, uint) -> uint { return x /= y; }
operator % (uint, uint) -> uint { return x %= y; }

operator | (uint, uint) -> uint { return x |= y; };
operator & (uint, uint) -> uint { return x &= y; };
operator ^ (uint, uint) -> uint { return x ^= y; };

operator << (uint, uint) -> uint { return x <<= y; };
operator >> (uint, uint) -> uint { return x >>= y; };
