#include <iostream>
#include <vector>

#include <utility>
#include <type_traits>

using yes = std::true_type;
using no = std::false_type;

#define TEST_HAS_NON_MEMBER(func) \
template<class... Args> \
struct has_non_member_##func { \
	template<class... Ty> \
	static auto check(int) -> decltype( func(std::declval<Ty&>()...), yes() ); \
\
	template<class... Ty> \
	static auto check(...) -> no; \
\
	static const bool value = std::is_same<decltype( check<Args...>(0) ), yes>::value;\
};

#define TEST_HAS_MEMBER(func) \
template<class C, class... Args> \
struct has_member_##func { \
	template<class T, class... Ty> \
	static auto check(int) -> decltype( std::declval<T&>().func(std::declval<Ty&>()...), yes() ); \
\
	template<class T, class... Ty> \
	static auto check(...) -> no; \
\
	static const bool value = std::is_same<decltype( check<C, Args...>(0) ), yes>::value;\
};

TEST_HAS_NON_MEMBER(archive)
TEST_HAS_NON_MEMBER(save)
TEST_HAS_NON_MEMBER(load)
TEST_HAS_MEMBER(archive)
TEST_HAS_MEMBER(save)
TEST_HAS_MEMBER(load)

#define HAS_NON_MEMBER(func) has_non_member_##func<A, T>::value
#define HAS_MEMBER(func) has_member_##func<T, A>::value

enum class dummy {};

static const dummy mad = {};

template<bool expr>
using EnableIf = typename std::enable_if<expr, dummy>::type;
#define ENABLE_IF(expr) EnableIf<expr> = mad

template <typename A>
class OutputArchive {
public:
	OutputArchive(A& derived)
		: derived(derived)
	{
	}

	A& derived;

	template<typename T>
	void process(T& value, char const* name)
	{
		start(derived, name, value);
		call_archive(value);
		end(derived, name, value);
	}


	class Proxy {
	public:
		Proxy(OutputArchive& arc, char const* name)
			: arc(arc), name(name)
		{
		}

		template<typename T>
		void operator - (T& value)
		{
			arc.process(value, name);
		}
	private:
		OutputArchive& arc;
		char const* name;
	};

	Proxy operator - (char const* name)
	{
		return Proxy(*this, name);
	}

private:
	template<typename T, ENABLE_IF(HAS_MEMBER(archive))>
	void call_archive(T& value)
	{
		value.archive(derived);
	}

	template<typename T, ENABLE_IF(HAS_NON_MEMBER(archive))>
	void call_archive(T& value)
	{
		archive(derived, value);
	}

	template<typename T, ENABLE_IF(HAS_MEMBER(save))>
	void call_archive(T& value)
	{
		value.save(derived);
	}

	template<typename T, ENABLE_IF(HAS_NON_MEMBER(save))>
	void call_archive(T& value)
	{
		save(derived, value);
	}
};

template <typename A>
class InputArchive {
public:
	InputArchive(A& derived)
		: derived(derived)
	{
	}

	A& derived;

	template<typename T>
	void process(T& value, char const* name)
	{
		start(derived, name, value);
		call_archive(value);
		end(derived, name, value);
	}

	class Proxy {
	public:
		Proxy(InputArchive& arc, char const* name)
			: arc(arc), name(name)
		{
		}

		template<typename T>
		void operator - (T& value)
		{
			arc.process(value, name);
		}
	private:
		InputArchive& arc;
		char const* name;
	};

	Proxy operator - (char const* name)
	{
		return Proxy(*this, name);
	}

private:
	template<typename T, ENABLE_IF(HAS_MEMBER(archive))>
	void call_archive(T& value)
	{
		value.archive(derived);
	}

	template<typename T, ENABLE_IF(HAS_NON_MEMBER(archive))>
	void call_archive(T& value)
	{
		archive(derived, value);
	}

	template<typename T, ENABLE_IF(HAS_MEMBER(load))>
	void call_archive(T& value)
	{
		value.load(derived);
	}

	template<typename T, ENABLE_IF(HAS_NON_MEMBER(load))>
	void call_archive(T& value)
	{
		load(derived, value);
	}
};



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


struct const_str {
private:
	const char* ptr;
	const std::size_t len;
public:
	template<std::size_t N>
	constexpr const_str(const char(&a)[N])
		: ptr(a), len(N-1)
	{}

	constexpr char operator[] (const std::size_t n)
	{
		//static_assert(n < len, "Out of range");
		return ptr[n];
	}

	constexpr std::size_t size()
	{
		return len;
	}
};

template <typename T, ENABLE_IF(IsFloat<T>)>
constexpr auto typeName(T) -> char const* {
	return "float";
};

template <typename T, ENABLE_IF(IsBool<T>)>
constexpr auto typeName(T) -> char const* {
	return "bool";
};

template <typename T, ENABLE_IF(IsString<T>)>
constexpr auto typeName(T) -> char const* {
	return "string";
};

template <typename T, ENABLE_IF(IsInt<T>)>
constexpr auto typeName(T) -> char const* {
	return "int";
};

struct Couter : OutputArchive<Couter> {
	size_t indent = 0;

	Couter()
		: OutputArchive<Couter>(*this)
	{
		std::cout << std::boolalpha;
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

	template<typename T, ENABLE_IF(!IsPrimitive<T>)>
	friend void start(Couter& arc, char const* name, T& value)
	{
		arc.printIndent();
		std::cout << "[" << name << std::endl;
		arc.addIndent();
	}

	template<typename T, ENABLE_IF(!IsPrimitive<T>)>
	friend void end(Couter& arc, char const* name, T& value)
	{
		arc.removeIndent();
		arc.printIndent();
		std::cout << "]" << std::endl;
	}

	template<typename T, ENABLE_IF(IsPrimitive<T>)>
	friend void start(Couter& arc, char const* name, T& value)
	{
		arc.printIndent();
		std::cout << name << " = " << typeName(value) << ": ";
	}

	template<typename T, ENABLE_IF(IsPrimitive<T>)>
	friend void end(Couter& arc, char const* name, T& value)
	{
	}

};

template<typename T, ENABLE_IF(IsPrimitive<T> && !IsString<T>)>
void archive(Couter& arc, T& x)
{
	std::cout << x << std::endl;
}

template<typename T, ENABLE_IF(IsString<T>)>
void archive(Couter& arc, T& x)
{
	std::cout << '"' << x << '"' << std::endl;
}



struct skip {
	std::string text;
	skip(const char * text) : text(text) {}
	skip(const char ch) : text(1,ch) {}
	skip(std::string text) : text(text) {}
};

struct ex {
	std::string w;
	ex(std::string w) : w(w) {}
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
		if (c != *text)
			throw ex(""s + "'" + *text + "'" + " / '" + c + "'");
		++text;
	}

	stream.flags(f);
	return stream;
}


struct Cinner : InputArchive<Cinner> {
	size_t indent = 0;

	Cinner()
		: InputArchive<Cinner>(*this)
	{
		//std::cin >> std::boolalpha;
	}

	template<typename T, ENABLE_IF(!IsPrimitive<T>)>
	friend void start(Cinner& arc, char const* name, T& value)
	{
		std::cin >> skip("[") >> skip(name);
	}

	template<typename T, ENABLE_IF(!IsPrimitive<T>)>
	friend void end(Cinner& arc, char const* name, T& value)
	{
		std::cin >> skip("]");
	}

	template<typename T, ENABLE_IF(IsPrimitive<T>)>
	friend void start(Cinner& arc, char const* name, T& value)
	{
		std::cin >> skip(name) >> skip("=") >> skip(typeName(value)) >> skip(":");
	}

	template<typename T, ENABLE_IF(IsPrimitive<T>)>
	friend void end(Cinner& arc, char const* name, T& value)
	{
	}

};

template<typename T, ENABLE_IF(IsPrimitive<T> && !IsString<T>)>
void archive(Cinner& arc, T& x)
{
	std::cin >> x;
}

template<typename T, ENABLE_IF(IsString<T>)>
void archive(Cinner& arc, T& x)
{
	std::cin >> skip('"') >> x >> skip('"');
}



struct vec3 {
	template<typename Archive>
	void archive(Archive& arc)
	{
		arc - "x" - vec[0];
		arc - "y" - vec[1];
		arc - "z" - vec[2];
	}

	float vec[3];
};

struct mat3 {
	template<typename Archive>
	void archive(Archive& arc)
	{
		arc - "v" - vec[0];
		arc - "v" - vec[1];
		arc - "v" - vec[2];
	}

	vec3 vec[3];
};
/*
struct dum {};
template<typename Archive>
Archive& arc operator - (Archive& arc, dum du)
{
	return arc;
}*/

#define _(T) #T - T

struct test1 {
	template<class Archive>
	void save(Archive& arc) const
	{
		int x = 1337;
		arc - "x" - x;
	}

	template<class Archive>
	void load(Archive& arc)
	{
		int x = 1337;
		arc - "x" - x;
	}
};

int main()
{
	vec3 v3{0.57, 1.23, 1.00001};
	Couter cou;
	
	cou - "vec" - v3;

	test1 t;
	cou - _(t);

	mat3 m3{{
		{1,0,0},
		{0,1,0},
		{0,0,1}
	}};

	cou - "mat" - m3;

	std::string str = "haiiir";
	int rofl = 5;
	bool bol = true;
	short shore = 65534;
	double flot = 1.07e99;

	cou -_ (str);
	cou -_ (rofl);
	cou -_ (bol);
	cou -_ (shore);
	cou -_ (flot);

	int x;
	Cinner ci;
	mat3 mat;
	try {
		ci - "x" - x;
	} catch(ex x) {
		std::cout << "fuck: " << x.w << std::endl;
	}

	std::cout << x << std::endl;
	try {
		ci - "mat" - mat;
	} catch(ex x) {
		std::cout << "fuck: " << x.w << std::endl;
	}
	cou - "mat" - mat;
}


/*
class Class : public Archivable {
public:
	Class(float x, float y, int num)
		:   x(x),    y(y),  num(num)
	{
	}

	virtual void archive(Archiver& arc)
	{
		arc - "x"   - x;
		arc - "y"   - y;
		arc - "num" - num;
	}
private:
	float x;
	float y;
	int num;
};

class Cliss {
public:
	Cliss(float x, float y, int num)
		: c(x,y,num), name("paiben")
	{
	}

	template<typename Archiver>
	void archive(Archiver& arc)
	{
		arc-"name"-name;
		arc-"c"-c;
	}

private:
	Class c;
	std::string name;
};

class Cross : public Archivable {
public:
	Cross(Cliss* cliss)
		: cl(cliss)
	{
	}

	virtual void archive(Archiver& arc)
	{
		arc - "HAHAHA" - cl;
	}
private:
	Cliss* cl;
};

struct Gross {
void archive(Archiver& arc)
{
}
};

struct vec3 {
float xyz[3];
};

void archive(Archiver& arc, vec3& value)
{
	arc - "x" - value.xyz[0];
	arc - "y" - value.xyz[1];
	arc - "z" - value.xyz[2];
}


#include <experimental/type_traits>
template<class T, typename... Args>
using member_archive = decltype( std::declval<T&>().archive(std::declval<Args&>()...) );

template<class T, typename... Args>
using has1_member_archive = std::experimental::is_detected<T, Args..., member_archive>;

template<typename A, typename T>
void tests()
{
	std::cout << HAS_NON_MEMBER(archive) << std::endl;
	std::cout << HAS_MEMBER(archive) << std::endl;
}

int main()
{
	Couter arc;

	tests<Archiver, vec3>();
	tests<Archiver, Gross>();

	double huy = 21;
	float  yhu = 21.0;
	int yuh    = 21;
	arc-"huy"-huy;
	arc-"yhu"-yhu;
	arc-"yuh"-yuh;
	
	Cliss clazz(1.0, 2.0, 5);
	//arc-"clazz"-clazz;

	Cross cr(&clazz);
	//arc - "rz" - cr;

	vec3 v3{1.0, 2.0, 3.0};
	arc - "v3" - v3;
}

*/
