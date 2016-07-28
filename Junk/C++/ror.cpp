enum Category {
	CAT_0,
	CAT_1,
	CAT_2
};

struct coid_t {
	using type = void;
};

template<typename, typename = coid_t>
struct sfinae;

template<typename T>
struct sfinae<T,typename T::cat1> {
	static const Category cat = CAT_1;
};

template<typename T>
struct sfinae<T,typename T::cat2> {
	static const Category cat = CAT_2;
	using type1 = typename T::cat2;
	using type2 = typename type1::type;
};

struct item {
	typedef coid_t cat1;
	typedef int cat2;
};

struct npc {
	typedef int cat1;
	typedef coid_t cat2;
};

#include <iostream>
int main()
{
	std::cout << sfinae<item>::cat << "\n";
	std::cout << sfinae<npc>::cat << "\n";
}
