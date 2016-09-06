#include <vector>
#include <deque>
#include <iostream>

struct TypeTag {

};

template <class T, template<class, class...> class C>
void print(C<T>& con)
{
	std::cout << "vector:{";
	for (auto& o : con) {
		std::cout << "[" << o << "],";
	}
	std::cout << "}" << std::endl;;
}

template <template<class, class...> class C>
void print(C<int>& con)
{
	std::cout << "vector<int>:{";
	auto beg = std::begin(con);
	auto end = std::end(con);
	
	if (beg < end) {
		std::cout << *beg++;
		for (; beg != end; ++beg) {
			std::cout << "," << *beg;
		}
	}
	std::cout << "}" << std::endl;;
}

template <template<class, class...> class C>
void print(C<float>& con)
{
	std::cout << "vector<float>:{";
	for (auto& o : con) {
		std::cout << "" << o << ",";
	}
	std::cout << "}" << std::endl;;
}

template <template<class, class...> class C>
void print(C<double>& con)
{
	std::cout << "vector<double>:{";
	for (auto& o : con) {
		std::cout << "" << o << ",";
	}
	std::cout << "}" << std::endl;
}

struct ct {
	void operator()(float x, float y, float z)
	{
		std::cout << ": " << x << ' ' << y << ' ' << z << std::endl;
	}
};

struct dummy {};

ct operator + (dummy d, char const* name)
{
	std::cout << name;
	return ct();
}

int main()
{
	std::vector<int> e;
	std::vector<int> v{0,0,1,1,1,2,2,2};
	std::vector<float> f{0,0,1,1,1,2,2,2};
	std::deque<float> d{0.0,0.1,0.2,0.3,0.3,0.5};

	print(e);
	print(v);
	print(f);
	print(d);

	dummy dum;

	(dum + "yuh") (1,2,3);
}
