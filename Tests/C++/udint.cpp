#include <iostream>

class Short;
class Int;
class Long;
class Float;
class Double;

class Short {
	friend class Int;
	friend class Long;
	friend class Float;
	friend class Double;
	short value;
public:
	Short() = default;
	Short(Short const& other);
	Short(Int   const& other);
	Short(Float const& other);

	Short& operator+=(Short other);
	Short& operator+=(Int   other);
	Short& operator+=(Float other);

	friend std::ostream& operator<<(std::ostream& os, Short value)
	{
		return os << value.value;
	}
};

class Int {
	friend class Short;
	friend class Long;
	friend class Float;
	friend class Double;
	int value;
public:
	friend Int operator""_i(unsigned long long x);

	Int() = default;
	Int(Short const& other);
	Int(Int   const& other);
	Int(Float const& other);

	Int& operator+=(Short other);
	Int& operator+=(Int   other);
	Int& operator+=(Float other);

	friend std::ostream& operator<<(std::ostream& os, Int value)
	{
		return os << value.value;
	}
};

Int operator""_i(unsigned long long x)
{
	Int i;
	i.value = x;
	return i;
}

class Float {
	friend class Short;
	friend class Int;
	friend class Long;
	friend class Double;
	float value;
public:
	friend Float operator""_f(long double x);

	Float() = default;
	Float(Short const& other);
	Float(Int   const& other);
	Float(Float const& other);
	

	Float& operator+=(Short other);
	Float& operator+=(Int   other);
	Float& operator+=(Float other);

	friend std::ostream& operator<<(std::ostream& os, Float value)
	{
		return os << value.value;
	}
};

Float operator""_f(long double x)
{
	Float f;
	f.value = x;
	return f;
}


Short::Short(Short const& other) : value(other.value) {}
Short::Short(Int   const& other) : value(other.value) {}
Short::Short(Float const& other) : value(other.value) {}

Short& Short::operator+=(Short other) { value += other.value; return* this; }
Short& Short::operator+=(Int   other) { value += other.value; return* this; }
Short& Short::operator+=(Float other) { value += other.value; return* this; }

Int::Int(Short const& other) { value = other.value; }
Int::Int(Int   const& other) { value = other.value; }
Int::Int(Float const& other) { value = other.value; }

Int& Int::operator+=(Short other) { value += other.value; return* this; }
Int& Int::operator+=(Int   other) { value += other.value; return* this; }
Int& Int::operator+=(Float other) { value += other.value; return* this; }

Float::Float(Short const& other) : value(other.value) {}
Float::Float(Int   const& other) : value(other.value) {}
Float::Float(Float const& other) : value(other.value) {}

Float& Float::operator+=(Short other) { value += other.value; return *this; }
Float& Float::operator+=(Int   other) { value += other.value; return *this; }
Float& Float::operator+=(Float other) { value += other.value; return *this; }


Int operator+(Short a, Short b) { return a+=b; }
Int operator+(Short a, Int b)   { return b+=a; }
Int operator+(Int a, Short b)   { return a+=b; }
Int operator+(Int a, Int b)     { return a+=b; }

Float operator+(Short a, Float b) { return b+=a; }
Float operator+(Float a, Short b) { return a+=b; }
Float operator+(Int   a, Float b) { return b+=a; }
Float operator+(Float a, Int   b) { return a+=b; }
Float operator+(Float a, Float b) { return a+=b; }

int main()
{
	Short s = 200_i;
	Int   i = 100_i;
	Float f = 300.0_f;

	std::cout << i + s << '\n';
	std::cout << f + s << '\n';
	std::cout << f + i << '\n';


	i += f;
	s = s + f;

	s = i + f;

	std::cout << i + s << '\n';
	std::cout << f + s << '\n';
	std::cout << f + i << '\n';
}
