#include <map>
#include <utility>
#include <typeinfo>
#include <iostream>
#include <typeindex>
#include </home/awdev/awlib/include/aw/utility/static_object.h>

namespace aw {
template <class CreatorFunc>
struct ClassRegistry;

template <class Base, typename... Args>
struct ClassRegistry<Base*(Args...)> {
	using CreatorFunc = Base*(Args...);

	using Map = std::map<std::string, CreatorFunc*>;

	template<class T>
	static bool add()
	{
		static Map& map = aw::static_object<Map>::instance();

		map[T::class_name] = [] (Args... args) -> Base*
		{
			return new T(std::forward<Args>(args)...);
		};

		return true;
	}
};
} // namespace aw

template <class T, class CreatorFunc>
struct Register;

template <class T, class B, class... Args>
struct Register<T,B*(Args...)> {
	static bool const b;

	void use(bool const&) {}
	Register()
	{
		use(b);
	}
};
template <class T, class B, class... Args>
bool const Register<T,B*(Args...)>::b = aw::ClassRegistry<B*(Args...)>::template add<T>();

struct Class {
	static constexpr char const* class_name = "Class";

	virtual char const* className() const
	{
		return class_name;
	}

	Class(std::string mas)
		: msg(mas)
	{}

	virtual void t()
	{
		std::cout << msg << " WHEEEE\n";
	}

	std::string msg;
};

struct Derived : Class {
	static constexpr auto class_name = "Derived";

	using Class::Class;

	virtual char const* className() const
	{
		return class_name;
	}

	virtual void t()
	{
		std::cout << msg << " WHOOOO\n";
	}
};

#define AW_DECLARE_CLASS_NAME(Class) \
	static constexpr auto class_name = #Class; \
	virtual auto className() const() { return class_name; }

#define AW_REGISTER_CLASS(Class, Base, ...) \
namespace aw {\
namespace _impl {\
	Register<Class, Base*(__VA_ARGS__)> Class##_reg;\
}} // namespace

AW_REGISTER_CLASS(Class,   Class,   std::string);
AW_REGISTER_CLASS(Derived, Class,   std::string);

int main()
{
	Class c("");

	using CreatorFunc = Class*(std::string);
	using Map = std::map<std::string, CreatorFunc*>;
	Map& map = aw::static_object<Map>::instance();
	
	Class* cc = map[Class::class_name]("aaa");
	cc->t();
	Class* dd = map[Derived::class_name]("aaa");
	dd->t();
}
