#include <vector>
#include <list>
#include <type_traits>


struct _impl {
struct component_id_dummy {};
}

struct component_id_type {
	_impl::component_id_dummy* id;
};

#define aw_register_component_type \
	static inline const ::_impl::component_id_dummy _component_id_dummy

template<typename T>
constexpr component_id_type component_id{ &T::_component_id_dummy };

struct bulshit {
	aw_register_component_type;
};


#include <iostream>
template <const int* T>
struct registrar {
    void echo() { std::cout<<T;} 
};

registrar<ptr>r;

int main()
{
 r.echo();
}
