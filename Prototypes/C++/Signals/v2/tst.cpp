#include "memfun.h"
#include <aw/types/support/reinterpret.h>
#include <iostream>

using namespace aw;

class unknown;
template<typename R, typename... Args>
using unknown_mem_fun = R(unknown::*)(Args...);

template<typename... Args>
using storage = typename std::aligned_storage<
	sizeof(unknown_mem_fun<void,Args...>),
	alignof(unknown_mem_fun<void,Args...>)
>::type;

template<class T, typename...Args>
void invoke(storage<Args...>& data, void* c, Args... args)
{
	auto func = aw::reinterpret_any<member_func<T,void(Args...)>>(data);
	T* ptr = (T*)c;
	(ptr->*func)(args...);
}

template<typename...Args>
struct func {
	using storage_type = storage<Args...>;
	using invoker_type = void(*)(storage_type&, void*, Args...);

	template<typename T>
	func(T* t, member_func<T,void(Args...)> func)
	{
		stor = aw::reinterpret_any<storage_type>(func);
		invoker = invoke<T,Args...>;
		receiv = t;
	}

	void operator()(Args... args)
	{
		invoker(stor, receiv, args...);
	}

	void* receiv;
	invoker_type invoker;
	storage_type stor;
};

struct D {
	void cou()
	{
		std::cout << "Hullo\n";
	}
};

struct C {
	virtual void cou()
	{
		std::cout << "HOLOU\n";
	}
};

struct B : C,D {
	virtual void cou()
	{
		std::cout << "Hola!\n";
	}
	
	char c;
};

struct A0 {
	void cou()
	{
		std::cout << "YOB!\n";
	}
};

struct A1:A0{};
struct A2:A0{};

struct A : virtual A1, virtual A2 {
	void cou()
	{
		std::cout << "YOB!\n";
	}
};


#include <functional>
int main()
{
	D coutor;
	C ctor;
	B cou;
	A cu;
	func<> lail{&coutor, &D::cou};
	func<> loul{&ctor, &C::cou};
	func<> lolo{&cou, &B::cou};
	func<> lool{&cu, &A::cou};

	void* cup = &coutor;
	std::function<void(void*)> f  {&D::cou};

	f(cup);

	lail();
	loul();
	lolo();
	lool();
}
