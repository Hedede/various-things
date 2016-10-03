int x = 2;
auto lamb = [] { return x; };
struct ss : decltype(lamb) {
	ss(int x, int y, int z)
		: decltype(lamb){ lamb }, x{x}, y{y}, z{z}
	{
	}
	int operator()()
	{
		auto w = decltype(lamb)::operator()();
		return x + y*w + z*w;
	}
	int x, y, z;
};


int main() 
{
	ss* s = new ss{3,2,1};
	return (*s)();
}
