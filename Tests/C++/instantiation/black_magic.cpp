#include <iostream>
#include <vector>


/***********************************************/
template <class T> //, class Tag = EmptyTag>
class StaticObject {
	static T& ref;

	static void use(T const&) {}

	static T& create()
	{
		static T object;
		use(ref);
		return object;
	}

public:
	static T& instance()
	{
		return create();
	}

	static T const& const_instance()
	{
		return create();
	}
};

template <class T>
T& StaticObject<T>::ref = StaticObject<T>::create();
/***********************************************/

template <class T>
struct init_binding;

template <class T>
struct binding_name {};

template <class T>
struct bind_to_archives {
	bind_to_archives const & bind() const
	{
		instantiate_polymorphic_binding((T*) 0, 0);
		return *this;
	}
};

template <class T>
void instantiate_polymorphic_binding(T*, int) {}

#define REGTY(T)                                        \
  template <>                                           \
  struct binding_name<T>                                \
  {                                                     \
    static char const * name() { return #T; } \
  };                                                    \
    template<>                                       \
    struct init_binding<T> {                         \
        static bind_to_archives<T> const & b;        \
        static void unused() { (void)b; }            \
    };                                               \
    bind_to_archives<T> const & init_binding<T>::b = \
        StaticObject<bind_to_archives<T> >::instance().bind()

#define REGA(A)                              \
  template <>                                           \
  struct binding_name<A>                                \
  {                                                     \
    static char const * name() { return #A; } \
  };                                                    \
template <class T> \
typename instantiator<push_backer<A, T>>::type          \
instantiate_polymorphic_binding( T*, A*)

template <void(*)()>
struct instantiate_function {};

template <class T>
struct instantiator {
	typedef int type;

	static void instantiate() //__attribute__ ((__used__)) ;
	{
		StaticObject<T>::instance();
	}

	virtual void dummy1() { instantiate(); }
	using        dummy2   = instantiate_function<instantiate>;
};

/*
template <class A, class T>
void polymorphic_serialization_support<A,T>::instantiate()
*/

typedef StaticObject<std::vector<std::string> > MAP;


template<typename A, class T>
struct push_backer {
	push_backer()
	{
		std::string s  = binding_name<A>::name();
		s += " ";
		s += binding_name<T>::name();
		MAP::instance().push_back(s);
	}
};


struct A1 {};
REGA(A1);
struct A2 {};
REGA(A2);

struct T1 {};
struct T2 {};
struct T3 {};
REGTY(T1);
REGTY(T2);
REGTY(T3);

int main()
{
	for (int i = MAP::instance().size()-1; i >= 0; --i)
		std::cout << MAP::instance()[i] <<"\n";
	//for(auto s: map)
		//std::cout << s <<"\n";
}
