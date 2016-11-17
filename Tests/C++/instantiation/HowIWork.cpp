template <class T> //, class Tag = EmptyTag>
class Singleton {
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
T& Singleton<T>::ref = Singleton<T>::create();

template <void(*)()>
struct instantiate_function {};

template <class T>
struct Instantiator {
	typedef int type;

	static void instantiate() //__attribute__ ((__used__)) ;
	{
		Singleton<T>::instance();
	}

	virtual void dummy1() { instantiate(); }
	using dummy2 = instantiate_function<instantiate>;
};

template <class T> void adl_magic(T*, int) {}

template <class T>
struct Magic {
	Magic const& magic() const
	{
		adl_magic(static_cast<T*>(nullptr), 0);
		return *this;
	}
};

#include <iostream>

template<class A, class B>
struct Test {
	Test()
	{
		std::cout << A::name << ' ' << B::name << "\n";
	}
};

struct Dummy1 {constexpr static auto name = "Dummy1";};
struct Dummy2 {constexpr static auto name = "Dummy2";};
struct Dummy3 {constexpr static auto name = "Dummy3";};

template <class T> typename Instantiator<Test<Dummy1, T>>::type adl_magic(T*, Dummy1*);
template <class T> typename Instantiator<Test<Dummy2, T>>::type adl_magic(T*, Dummy2*);
template <class T> typename Instantiator<Test<Dummy3, T>>::type adl_magic(T*, Dummy3*);

Magic<Dummy1> r1 = Singleton<Magic<Dummy1>>::instance().magic();
Magic<Dummy2> r2 = Singleton<Magic<Dummy2>>::instance().magic();
Magic<Dummy3> r3 = Singleton<Magic<Dummy3>>::instance().magic();

int main() { }
