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
TEST_HAS_MEMBER(archive)

#define HAS_NON_MEMBER(func) has_non_member_##func<A, T>::value
#define HAS_MEMBER(func) has_member_##func<T, A>::value

enum class dummy {};

static const dummy mad = {};

template<bool expr>
using EnableIf = typename std::enable_if<expr, dummy>::type;




template <typename A>
class Archiver {
public:
	Archiver(A& der)
		: this_(der)
	{
	}

	A& this_;

	template<typename T, EnableIf<HAS_MEMBER(archive)> = mad>
	void process(T& value, char const* name)
	{
		start(this_, name, value);
		value.archive(this_);
		end(this_, name, value);
	}

	template<typename T, EnableIf<HAS_NON_MEMBER(archive)> = mad>
	void process(T& value, char const* name)
	{
		start(this_, name, value);
		archive(this_, value);
		end(this_, name, value);
	}

	class Proxy {
	public:
		Proxy(Archiver& arc, char const* name)
			: arc(arc), name(name)
		{
		}

		template<typename T>
		void operator - (T& value)
		{
			arc.process(value, name);
		}
	private:
		Archiver& arc;
		char const* name;
	};

	Proxy operator - (char const* name)
	{
		return Proxy(*this, name);
	}
};

struct Couter : Archiver<Couter> {
	size_t indent = 0;

	Couter()
		: Archiver<Couter>(*this)
	{
	}

	template<typename T>
	friend void start(Couter& arc, char const* name, T& value)
	{
		if (arc.indent > 0)
			std::cout << std::string(4*arc.indent, ' ');
		std::cout << "[" << name << std::endl;
		++arc.indent;
	}

	template<typename T>
	friend void end(Couter& arc, char const* name, T& value)
	{
		if (arc.indent > 0)
			--arc.indent;
		if (arc.indent > 0)
			std::cout << std::string(4*arc.indent, ' ');
		std::cout << "]" << std::endl;
	}

	friend void start(Couter& arc, char const* name, float& value)
	{
		if (arc.indent > 0)
			std::cout << std::string(4*arc.indent, ' ');
		std::cout << name << " = float: ";
	}

	friend void end(Couter& arc, char const* name, float& value)
	{
	}

};

void archive(Couter& arc, float x)
{
	std::cout << x << std::endl;
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

#include <cstdio>

int main()
{
	std::freopen("output.txt","w",stdout);

	vec3 v3{0.57, 1.23, 1.00001};
	Couter cou;
	
	cou - "vec" - v3;

	mat3 m3{{{1,0,0},{0,1,0},{0,0,1}}};

	cou - "mat" - m3;
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

