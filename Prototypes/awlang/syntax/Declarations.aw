// function declarations:

     typename  func1  ( type arg1, arg2, ... );              // [2]
func typename  func2  ( type arg1, arg2, ... );              // [1]
func           func3  ( type arg1, arg2, ... );              // [3]
func           func4  ( type arg1, arg2, ... ) - > typename; // [4]

// equivalent in cpp:
typename  func1  ( arg1, arg2, ... );              // [1,2]
auto      func3  ( arg1, arg2, ... );              // [3]
auto      func4  ( arg1, arg2, ... ) - > typename; // [4]


// variable declarations:
    typename var1; // [1]
var typename var2; // [2]
var          var3; // [3]

// equivalent in cpp:
typename var1; // [1]
typename var2; // [2]
auto     var3; // [3]


// class declarations:
class class1;                         // [1]
class class2                     { }  // [2]
class class3 (base1, base2, ...) { }  // [3]

// equivalent in cpp:
struct class1                          // [1]
struct class2                     { }; // [2]
struct class2 : base1, base2, ... { }; // [3]


// variable initialization:
// Now, I'm unsure: I like
struct S {
    int x;
    struct Foo {
        int i;
        int j;
        int a[3];
    } b;
};

S s1 = { 1, { 2, 3, {4, 5, 6} } };
S s3 = { 1,   2, 3,  4, 5, 6    };
S s2   { 1, { 2, 3, {4, 5, 6} } };
S s4   { 1,   2, 3,  4, 5, 6    };
// syntax (those are actual examples from cpp and all same)
// or,

struct Vec {
	float x,y,z;
	using T = float;
	Vec(T x, T y, T z)
		: x{x}, y{y}, z{z}
	{ }
};

Vec = {1.0, 2.0, 3.0};

// but there's also constructors which don't initialize elements directly:

auto streeng = std::string( line_length, '=' );
// "=========================================="

// what if…

class Vec {
	var x, y, z;

	// initializes elements as named, in that order
	Vec {var x, var y, var z};

	Vec (var radius, var angle)
	{
		x = radius * cos(angle);
		y = radius * sin(angle);
		z = oops z, segodnya ne tvoj den;
	}
};

Class vec = {x,y,z};
Class vec = Class(r, a);

// or... lol, DO IT SIMILAR TO LAMBDA SYNTAX! but slightly rearranged:
// *tries* no, nevermind

class Vec {
	var x, y, z;

	Vec [ init-list ] { func-body };           // Vec v = {x, y, z}
	Vec ( args ) [ init-list ] { func-body };  // Vec v(a, b)

	//ugh…

	Vec ( args ) : init-list { func-body };

	//hmm, what about named constructors…
	//like, instead of

	static polar(radius, angle) //var types ommited out of laziness
	{
		x = radius * cos(angle);
		y = radius * sin(angle);
		return Vec2(x, y);
	}


	polar ( radius, angle )
		: x(radius*cos), y(radius*sin)
	{} // Vec2 v = Vec2::polar( r, a )


	//okay, back to {} and ()

	Vec { args } : initlist { body ?}
	Vec ( args ) : initlist { body ?}

	//in first case initlist is redundant (bcos its, like, direct initialization)

	//ah yes, whats this all about:

	//i saw a quesion "which one to prefer: Class c(args) or Class c{args}"

	//answer was, like, prefer {} when it directly represents contents of a class, and prefer () when it *describes* contents 
	//like, string{"oooooooooooooooooooo"} vs string(20,'o')
};
