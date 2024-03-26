/* uint is implicitly defined with definition equivalent to this: */
constexpr size_t uint_size = /* natural cpu word size i.e. maximum size that can fit into the register */;

template<size_t size>
class uint {
	uint();
	uint(uint const&);

	template<size_t size2> requries { size2 <  size; }
	uint(uint<size2> const&);

	uint& operator=(uint const&);

	uint& operator+=(uint const&);
	uint& operator-=(uint const&);
	uint& operator*=(uint const&);
	uint& operator/=(uint const&);
	uint& operator%=(uint const&);

	uint& operator&=(uint const&);
	uint& operator|=(uint const&);
	uint& operator^=(uint const&);

	uint& operator<<=(uint const&);
	uint& operator>>=(uint const&);

	bool operator<(uint  const&)
	bool operator>(uint  const&)
	bool operator==(uint const&)
	bool operator!=(uint const&)
	bool operator<=(uint const&)
	bool operator>=(uint const&)

	bool         operator!();
	uint         operator~();
	uint         operator+();
	 int_t<size> operator-();
};

template<size_t size1, size_t size2>
operator uint(uint<size2> const&);

template<size_t size1, size_t size2>
operator uint(int<size2> const&);


uint operator+(uint, uint);
uint operator-(uint, uint);
uint operator/(uint, uint);
uint operator*(uint, uint);
uint operator%(uint, uint);

uint operator&(uint, uint);
uint operator|(uint, uint);
uint operator^(uint, uint);

uint operator<<(uint, size_t);
uint operator>>(uint, size_t);
