// There's 3 notions:
// implicit conversion
// explicit construction
// and explicit cast

// first one looks like:
int y;
int x = y;
bool b = x;

// second one looks like
int<16> y;
int x = int{y};

// third one looks like
int y;
int<16> x = int<16>(y);


// those are declared as:
class int {
	implicit int(int const&); // implicit is optional
	explicit int(int<smaller> const&);
};
// can't be declared inside class ?(unsure now)
implicit operator bool(int const&);
explicit operator int(uint const&); // explicit is optional
