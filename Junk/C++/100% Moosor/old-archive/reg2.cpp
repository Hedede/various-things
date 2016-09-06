#include <string>
#include <typeinfo>
#include <typeindex>
#include <iostream>
#include "enable_if.cpp"

AW_DECLARE_HAS_STATIC_VAR(classname);

template<class T>
constexpr auto classname = "<NO CLASS NAME>";

namespace aw {
namespace details {
template<typename T, class = void>
struct class_name;

template<typename T>
struct class_name<T, typename std::enable_if<has_static_classname<T>, void>::type> {
	static constexpr auto value = T::classname;
};

template<typename T>
struct class_name<T, typename std::enable_if<!has_static_classname<T>, void>::type> {
	static constexpr auto value = classname<T>;
};
}

template<typename T>
constexpr auto class_name = details::class_name<typename std::remove_reference<typename std::remove_pointer<T>::type>::type>::value;
}

using void_func = void (*) (void);

template<typename T, class... Args>
using function = T* (*) (Args...);

struct CreatorBase {
	void* operator()
};

template<class T, class... Args>
struct Creator : public CreatorBase {
	void* operator()(Args... args)
	{
		return new T (args...);
	}
};

struct Classdef {
	Classdef(Classdef&&) = default;

	template <typename T, class... Args>
	Classdef(Creator<T, Args...> creator)
		: idx(std::type_index(typeid(creator)))
	{
		className = aw::class_name<T>;
		create = static_cast<CreatorBase>(creator);
		std::cout << idx.name() << std::endl;
		std::cout << std::type_index(typeid(create)).name() << std::endl;
	}

	template <typename T, class... Args>
	T* construct(Args... args)
	{
		/*
		std::cout << std::string(25, '=') << std::endl;
		std::cout << std::type_index(typeid(typed_func)).name() << std::endl;
		std::cout << std::type_index(typeid(typed_func)).hash_code() << std::endl;
		std::cout << idx.name() << std::endl;
		std::cout << idx.hash_code() << std::endl;
		std::cout << (std::type_index(typeid(typed_func)) == idx) << std::endl;
		std::cout << std::string(25, '=') << std::endl;

		if (std::type_index(typeid(typed_func)) != idx)
			return nullptr;

		auto casted_func = reinterpret_cast<typed_func>(create);

		return casted_func(args...);*/
		return nullptr;
	}

	std::string className;
	CreatorBase create;
	std::type_index idx;
};

void coutc(int a, char b, std::string v)
{
	int d = v.size();
	if (d < a) {
		d = (a - d) / 2;
	}
	if (d <= 0) d = 1;
	std::cout << std::string (a, b) << std::endl;
	std::cout << std::boolalpha;
	std::cout << std::string (d, ' ') << v << std::endl;
	std::cout << std::string (a, b) << std::endl;
}

template<>
constexpr auto classname<class Object> = "Object";


struct Object {
	static Object* create()
	{
		return new Object();
	}

	Object()
	{
		std::string name = aw::class_name<decltype(this)>;
		coutc(25,'-',name);
	}
};


struct object {
	static constexpr auto classname = "object";

	static object* create(int x, char y)
	{
		return new object(x, y);
	}

	object(int a, char b);
};

object::object(int a, char b)
{
	std::string name = aw::class_name<decltype(this)>;
	coutc(a,b,name);
}

int main(int, char** x)
{
	Creator<object, int, char> c;
	Creator<Object, int, char> C;

	Classdef o(c);
	Classdef O(C);


	c(80,'+');
	//o.create = static_cast<void_func>(lambda);

	char const* z[] = {"", "25", "-"};

	if (!x[1] || !x[2])
		x = const_cast<char**>(z);

	o.construct<object>(std::stoi(x[1]), x[2][0]);
	O.construct<Object>(std::stoi(x[1]), x[2][0]);
}
