#include<iostream>
template<typename T>
void use(T const&) {}

template<typename T>
class static_object {
	static T& ref;
	static T& create()
	{
		static T object;
		use(ref);  // why it doesn't work without this line?
		return object;
	}

public:
	static T& instance() { return create(); }
};
template <class T>
T& static_object<T>::ref = static_object<T>::create();

template<auto function>
struct call_in_ctor { call_in_ctor() { function(); } };

auto lambda = [] {std::cout << "щущ\n";};


namespace _impl {
template<typename T>
struct instantiator {
	static auto instantiate() { return static_object<T>::instance(); }
};
} // namespace _impl

/*!
 * Forces instantiation of a static object when referenced.
 */
template<typename T>
using force_instantiation = decltype(_impl::instantiator<T>::instantiate());


struct stotec {
	stotec()
	{
		std::cout << "щащ\n";
	}
};

using x = force_instantiation<stotec>;

template<auto function>
struct call_on_init {
	using x = force_instantiation<call_in_ctor<function>>;
};



template struct call_on_init<+lambda>;

int main() {}
