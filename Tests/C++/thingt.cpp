#include <iostream>
#include <tuple>
#include <typeinfo>

class testclass {
public:
	int& set_i(int new_i)
	{
		i = new_i;
		return i;
	}

	void set_s(std::string new_s)
	{
		s = new_s;
	}	

	void print_state()
	{
		std::cout << i << s << std::endl;
	}
private:
	int i;

	std::string s;
};

template <typename... Types>
struct pack
{
    // this single type represents a collection of types,
    // as the template arguments it took to define it
};

template <typename... Args>
struct pack_iterator;

template <typename Head, typename... Tail>
struct pack_iterator<Head, Tail...> {
	typedef Head type;
	typedef pack_iterator<Tail...> next;
};

template <>
struct pack_iterator<> {
};

template<size_t i, typename T>
struct pack_element;

template<size_t i, typename Head, typename... Tail>
struct pack_element<i, pack<Head, Tail...> >
: pack_element<i - 1, pack<Tail...> > { };

template<typename Head, typename... Tail>
struct pack_element<0, pack<Tail...> >
{
	typedef Head type;
};





namespace detail
{
    template<typename T, typename F, int... Is>
    void for_each(T&& t, F f, std::integer_sequence<int, Is...>)
    {
        auto l = { (f(std::get<Is>(t)), 0)... };
    }
}

template<typename... Ts, typename F>
void for_each_in_pack(pack<Ts...> const& t, F f)
{
    detail::for_each(t, f, std::make_integer_sequence<int, sizeof...(Ts)>());
}


struct print {
template <typename T>
void operator()(T)
{
	std::cout << typeid(T).name() << std::endl;
}
};

template <class R, class... Args>
pack<R, Args...> deduce_signature(R(*)(Args...))
{
    return pack<R, Args...>();
}

template <class R, class T, class... Args>
pack<R, T&, Args...> deduce_signature(R(T::*)(Args...))
{
    return pack<R, T&, Args...>();
}

int main()
{
	auto p = deduce_signature(&testclass::set_i);
	for_each_in_pack(p, print());
}
