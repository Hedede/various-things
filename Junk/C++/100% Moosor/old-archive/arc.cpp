#include <iostream>
#include <vector>
#include <map>

#include <utility>
#include <type_traits>
#include <functional>

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

#include "enable_if.cpp"

AW_DECLARE_HAS_NON_MEMBER(archive);
AW_DECLARE_HAS_NON_MEMBER(save);
AW_DECLARE_HAS_NON_MEMBER(load);

AW_DECLARE_HAS_MEMBER(archive);
AW_DECLARE_HAS_MEMBER(save);
AW_DECLARE_HAS_MEMBER(load);

AW_DECLARE_HAS_STATIC_VAR(classname);

template<typename T>
struct IsIntHlp : std::integral_constant<
	bool,
	std::is_integral<T>::value
> { };
template<>
struct IsIntHlp<bool> : std::false_type { };

template<typename T>
constexpr auto IsInt = IsIntHlp<T>::value;

template<typename T>
struct IsFloatHlp : std::integral_constant<
	bool,
	std::is_floating_point<T>::value
> { };

template<typename T>
constexpr auto IsFloat = IsFloatHlp<T>::value;

template<typename>
struct IsStringHlp : std::false_type{ };
template<class CharT, class Traits, class Alloc>
struct IsStringHlp<std::basic_string<CharT, Traits, Alloc>> : std::true_type{ };

template<typename T>
constexpr auto IsString = IsStringHlp<T>::value;


template<typename>
struct IsBoolHlp : std::false_type{ };
template<>
struct IsBoolHlp<bool> : std::true_type{ };

template<typename T>
constexpr auto IsBool = IsBoolHlp<T>::value;

template<typename T>
struct IsPrimitiveHlp : std::integral_constant<
	bool,
	std::is_arithmetic<T>::value || IsString<T>
> {};
template<typename T>
constexpr auto IsPrimitive = IsPrimitiveHlp<T>::value;

template <typename T, ENABLE_IF(IsFloat<T>)>
constexpr auto typeName(T const&) -> char const* {
	return "float";
};

template <typename T, ENABLE_IF(IsBool<T>)>
constexpr auto typeName(T const&) -> char const* {
	return "bool";
};

template <typename T, ENABLE_IF(IsString<T>)>
constexpr auto typeName(T const&) -> char const* {
	return "string";
};

template <typename T, ENABLE_IF(IsInt<T>)>
constexpr auto typeName(T const&) -> char const* {
	return "int";
};

namespace aw {
template<typename T>
constexpr auto classname = "";

namespace details {
template<typename T, class = void>
struct class_name {
static constexpr auto value = "";
};

template<typename T>
struct class_name<T, EnableIf<has_static_classname<T> > > {
	static constexpr auto value = T::classname;
};

template<typename T>
struct class_name<T, EnableIf<!has_static_classname<T> > > {
	static constexpr auto value = classname<T>;
};
}

template<typename T>
constexpr auto class_name = details::class_name<typename std::decay<T>::type>::value;
}

using namespace aw;

struct OutputArchive {
	template<class T, ENABLE_IF(IsPrimitive<T>)>
	void operator()(char const* name, T const& value)
	{
		value_start(name);

		write(value);

		value_end(name);
	}

	template<class T, ENABLE_IF(!IsPrimitive<T> && !std::is_pointer<T>::value)>
	void operator()(char const* name, T const& value)
	{
		object_start(name);

		object_write(value);

		object_end(name);
	}

	template<class T>
	void operator()(char const* name, T const* value)
	{
		if (!value)
			return;

		auto type = value->classDef().className();
		object_start(name, type);

		polymorphic_write(value);

		object_end(name);
	}
private:
	virtual void object_start(char const* name) = 0;
	virtual void object_start(char const* name, char const* type) = 0;
	virtual void object_end(char const* name) = 0;


	template<typename T, EnableIf<has_member_save<T,OutputArchive>> = dummy>
	void object_write(T const& value)
	{
		value.save(*this);
	}

	template<typename T, EnableIf<has_non_member_save<OutputArchive,T>> = dummy>
	void object_write(T const& value)
	{
		save(*this, value);
	}

	template<typename T>
	void polymorphic_write(T const* value)
	{
		value->save(*this);
	}

	virtual void value_start(char const* name) = 0;
	virtual void value_end(char const* name)   = 0;

	virtual void write(char const& value) = 0;
	virtual void write(std::string const& value) = 0;

	/*
	virtual void write(i8 const& value) = 0;
	virtual void write(u8 const& value) = 0;

	virtual void write(i16 const& value) = 0;
	virtual void write(u16 const& value) = 0;

	virtual void write(i32 const& value) = 0;
	virtual void write(u32 const& value) = 0;

	virtual void write(i64 const& value) = 0;
	virtual void write(u64 const& value) = 0;

	virtual void write(f32 const& value) = 0;
	virtual void write(f64 const& value) = 0;*/


	friend void save(OutputArchive& arc, char const value) { arc.write(value); }
	friend void save(OutputArchive& arc, std::string const& value) { arc.write(value); }

	/*
	friend void save(OutputArchive& arc, i8& value) { arc.write(value); }
	friend void save(OutputArchive& arc, u8& value) { arc.write(value); }

	friend void save(OutputArchive& arc, i16& value) { arc.write(value); }
	friend void save(OutputArchive& arc, u16& value) { arc.write(value); }

	friend void save(OutputArchive& arc, i32& value) { arc.write(value); }
	friend void save(OutputArchive& arc, u32& value) { arc.write(value); }

	friend void save(OutputArchive& arc, i64& value) { arc.write(value); }
	friend void save(OutputArchive& arc, u64& value) { arc.write(value); }

	friend void save(OutputArchive& arc, f32& value) { arc.write(value); }
	friend void save(OutputArchive& arc, f64& value) { arc.write(value); }*/
};



class Couter : public OutputArchive {
	virtual void object_start(char const* name)
	{
		printIndent();
		std::cout << "[" << name << "\n";
		addIndent();
	}

	virtual void object_start(char const* name, char const* type)
	{
		printIndent();
		std::cout << "[" << name << " : " << type << "\n";
		addIndent();
	}

	virtual void object_end(char const* name)
	{
		removeIndent();
		printIndent();
		std::cout << "]" << "\n";
	}

	virtual void value_start(char const* name)
	{
		printIndent();
		std::cout << name << " = ";
	}

	virtual void value_end(char const* name) { }

	virtual void write(char const& value)
	{
		std::cout << typeName(value) << " : " << int{value} << "\n";
	}

	virtual void write(std::string const& value) 
	{
		std::cout << typeName(value) << " : " << value << "\n";
	}

	void printIndent() const
	{
		if (indent > 0)
			std::cout << std::string(4*indent, ' ');
	}

	void addIndent()
	{
		++indent;
	}

	void removeIndent()
	{
		if (indent > 0)
			--indent;
	}
	
	unsigned indent = 0;
};

template<typename... Args>
struct ArgsList {
	using Tuple = std::tuple<Args...>;
	ArgsList(Args... args)
		: values{args...}
	{
	}

	template<typename T, typename Func>
	void callFunc(T& ret, Func f)
	{
		constexpr auto size = std::tuple_size<Tuple>::value;

		callFunc2(ret, f, std::make_index_sequence<size>{});
	}

	template<typename T, typename Func, std::size_t... I>
	void callFunc2(T& ret, Func f, std::index_sequence<I...>)
	{
		ret = f(std::get<I>(std::forward<Tuple&&>(values))...);
	}

	Tuple values;
};

struct InputArchive {
	template<class T, ENABLE_IF(IsPrimitive<T>)>
	void operator()(char const* name, T& value)
	{
		value_start(name);

		read(value);

		value_end(name);
	}

	template<class T, ENABLE_IF(!IsPrimitive<T> && !std::is_pointer<T>::value)>
	void operator()(char const* name, T& value)
	{
		object_start(name);

		object_read(value);

		object_end(name);
	}
/*
	template<class T>
	void operator()(char const* name, T*& value)
	{
		object_start(name);

		auto type = read_type();
		auto classDef = T::ClassDef::map[type];

		value = classDef->create();

		polymorphic_read(value);

		object_end(name);
	}*/

	template<class T, typename... Args>
	void operator()(char const* name, T*& value, std::tuple<Args...>&& args)
	{
		static_assert(std::is_polymorphic<T>, "Pointer must be of polymorphic type!");

		object_start(name);

		auto type = read_type();
		auto classDef = T::ClassDef::findClassDef(type);

		if (classDef == nullptr) {
			std::cerr << "Could not find classdef!";
			return;
		}

		using Tuple = std::tuple<Args...>;
		constexpr auto size  = sizeof...(Args);
		constexpr auto index = std::make_index_sequence<size>{};

		construct(value, *classDef, std::forward<Tuple&&>(args), index);

		polymorphic_read(value);

		object_end(name);
	}

	template<class T, typename ClassDef, typename Tuple, std::size_t... I>
	void construct(T*& v, ClassDef& c, Tuple&& a, std::index_sequence<I...>)
	{
		v = c.create(std::get<I>(std::forward<Tuple&&>(a))...);
	}


private:
	virtual void object_start(char const* name) = 0;
	virtual void object_end(char const* name) = 0;


	template<typename T, EnableIf<has_member_load<T,InputArchive>> = dummy>
	void object_read(T& value)
	{
		value.load(*this);
	}

	template<typename T, EnableIf<has_non_member_load<InputArchive,T>> = dummy>
	void object_read(T& value)
	{
		load(*this, value);
	}

	template<typename T>
	void polymorphic_read(T* value)
	{
		value->load(*this);
	}

	virtual std::string read_type() = 0;

	virtual void value_start(char const* name) = 0;
	virtual void value_end(char const* name)   = 0;

	virtual void read(char& value) = 0;
	virtual void read(std::string& value) = 0;

	/*
	virtual void read(i8 const& value) = 0;
	virtual void read(u8 const& value) = 0;

	virtual void read(i16 const& value) = 0;
	virtual void read(u16 const& value) = 0;

	virtual void read(i32 const& value) = 0;
	virtual void read(u32 const& value) = 0;

	virtual void read(i64 const& value) = 0;
	virtual void read(u64 const& value) = 0;

	virtual void read(f32 const& value) = 0;
	virtual void read(f64 const& value) = 0;*/


	friend void load(InputArchive& arc, char& value) { arc.read(value); }
	friend void load(InputArchive& arc, std::string& value) { arc.read(value); }

	/*
	friend void load(InputArchive& arc, i8& value) { arc.read(value); }
	friend void load(InputArchive& arc, u8& value) { arc.read(value); }

	friend void load(InputArchive& arc, i16& value) { arc.read(value); }
	friend void load(InputArchive& arc, u16& value) { arc.read(value); }

	friend void load(InputArchive& arc, i32& value) { arc.read(value); }
	friend void load(InputArchive& arc, u32& value) { arc.read(value); }

	friend void load(InputArchive& arc, i64& value) { arc.read(value); }
	friend void load(InputArchive& arc, u64& value) { arc.read(value); }

	friend void load(InputArchive& arc, f32& value) { arc.read(value); }
	friend void load(InputArchive& arc, f64& value) { arc.read(value); }*/
};

struct skip {
	std::string text;
	skip(const char * text) : text(text) {}
	skip(const char ch) : text(1,ch) {}
	skip(std::string text) : text(text) {}
};

std::istream & operator >> (std::istream & stream, const skip & x)
{
	using namespace std::literals::string_literals;
	std::ios_base::fmtflags f = stream.flags();
	//stream >> std::noskipws;

	char c;
	auto text = x.text.begin();
	while (stream && *text) {
		stream >> c;
		std::cerr << c << "|";
		std::cerr << *text;
		if (c != *text) std::cerr << "fuck";
		/*
			std::string x = ""s + "'" + *text + "'" + " / '" + c + "'";
			std::cerr << "fuck: " << x << std::endl;
		}*/
		std::cerr << "\n";
		++text;
	}

	stream.flags(f);
	return stream;
}

class Cinner : public InputArchive {
	virtual void object_start(char const* name)
	{
		std::cin >> skip("[") >> skip(name);
	}

	virtual void object_end(char const* name)
	{
		std::cin >> skip("]");
	}

	virtual std::string read_type()
	{
		std::string type;
		std::cin >> skip(":");
		std::cin >> type;
		return type;
	}

	virtual void value_start(char const* name)
	{
		std::cin >> skip(name);
	}

	virtual void value_end(char const* name) {}

	virtual void read(char& value)
	{
		int val;
		std::cin >> skip("=") >> skip(typeName(value)) >> skip(":") >> val;
		value = val;
	}

	virtual void read(std::string& value)
	{
		std::cin >> skip("=") >> skip(typeName(value)) >> skip(":") >> value;
	}
};



template <class Base, class CreatorSignature>
struct ClassDef {
private:
	ClassDef               const* parent;
	std::string            const  name;

	std::function<CreatorSignature> const creator;
	using Creator = decltype(creator);
public:
	static ClassDef base(std::string name, Creator creator)
	{
		return ClassDef(nullptr, name, creator);
	}

	template<typename Parent>
	static ClassDef derived(std::string name, Creator creator)
	{
		return ClassDef(&Parent::classdef, name, creator);
	}

	char const* className() const
	{
		return name.c_str();
	}

	template<typename... Args>
	Base* create(Args... args) const
	{
		return creator(args...);
	}

	using ClassDefMapType = std::map<std::string, ClassDef*>;
	using ClassDefMap = StaticObject<ClassDefMapType>;

	static ClassDef* findClassDef(std::string const& name)
	{
		auto map = ClassDefMap::instance();
		auto iter = map.find(name);
		if (iter == std::end(map))
			return nullptr;
		return iter->second;
	}

private:
	ClassDef(ClassDef* parent, std::string name, Creator creator)
		: parent(parent), name(name), creator(creator)
	{
		ClassDefMap::instance()[name] = this;
	}
};

struct Base {
	using ClassDef = ::ClassDef<Base, Base*(char)>;

	static ClassDef classdef;
	virtual ClassDef& classDef() const
	{
		return classdef;
	}

	Base(char var1)
		: var1(var1)
	{}

	virtual void save(OutputArchive& arc) const
	{
		arc("var1", var1);
	}

	virtual void load(InputArchive& arc) = 0;

	char var1;
};
Base::ClassDef Base::classdef = Base::ClassDef::base("Base", nullptr);

void Base::load(InputArchive& arc)
{
	char x;
	arc("var1", x);
}

struct Derived : Base {
	using ClassDef = Base::ClassDef;

	static ClassDef classdef;
	virtual ClassDef& classDef() const
	{
		return classdef;
	}

	Derived(char var1)
		: Base(var1)
	{
	}

	virtual void save(OutputArchive& arc) const
	{
		Base::save(arc);
		arc("var2", var2);
	}

	virtual void load(InputArchive& arc)
	{
		Base::load(arc);
		arc("var2", var2);
	}
	
	char var2;
};

Derived::ClassDef Derived::classdef = Derived::ClassDef::derived<Base>(
	"Derived",
	[] (char c) -> Base* {
		return new Derived(c);
	}
);

struct Derived2 : Base {
	using ClassDef = Base::ClassDef;

	static ClassDef classdef;
	virtual ClassDef& classDef() const
	{
		return classdef;
	}

	Derived2(char var1)
		: Base(var1)
	{
	}

	virtual void save(OutputArchive& arc) const
	{
		Base::save(arc);
		arc("var2", var2);
	}

	virtual void load(InputArchive& arc)
	{
		Base::load(arc);
		arc("var2", var2);
	}
	
	char var2;
};

Derived2::ClassDef Derived2::classdef = Derived2::ClassDef::derived<Base>(
	"Derived2",
	[] (char c) -> Base* {
		return new Derived2(c);
	}
);

template<typename... Args>
ArgsList<Args...> make_arglist(Args... args)
{
	return ArgsList<Args...>(args...);
}

int main()
{
	Couter arc;

	Base* test  = 0;  //= new Derived(50);
	Base* test2 = 0; //= test->classDef().create(100);
	
/*	arc("test", test);
	arc("test2", test2);

	struct {
		void save(OutputArchive& arc) const
		{
			arc("x", x);
			arc("y", y);
			arc("z", z);
		}

		char x, y, z;
	} uniq;

	arc("object", uniq);

	arc("value", 'c');
	arc("value", std::string("char"));
*/

	

	Cinner arc2;

	arc2("test",  test,  std::make_tuple(111));
	arc2("test2", test2, std::make_tuple(99));

	arc("test",  test);
	arc("test2", test2);
}


