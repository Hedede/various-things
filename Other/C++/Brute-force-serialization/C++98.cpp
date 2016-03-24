template <class T>
class has_member_archive { // unused
	typedef char yes;
	typedef long no;

	template <class C> static yes test( typeof(&C::archive) ) ;
	template <class C> static no test(...);
public:
	enum { value = sizeof(test<T>(0)) == sizeof(yes) };
};

template <class T>
struct has_member_XXX {
	typedef char yes;
	typedef long no;

	struct Fallback { int XXX; };
	struct Derived : T, Fallback { };

	template <class C, C> struct ChT;

	template <class C> static yes (&test(ChT<int Fallback::*, &C::XXX>*));
	template <class C> static no (&test(...));

	enum { value = sizeof(test<Derived>(0)) == sizeof(yes) };
};

template <bool B, class T = void>
struct enable_if {};
 
template <class T>
struct enable_if <true, T> { typedef T type; };



/** Archive **/
#include <cassert>
#include <cstring>
#include <string>
#include <iostream>
struct Archive {
	Archive()
		: indent(0)
	{}

	std::string clean_name(char const* name)
	{
		std::string temp(name, std::strlen(name));

		if (indent > 0)
			assert(temp.substr(0,3) == "ar_" && "archive called on non-archivable thing!");

		if (temp.substr(0,3) == "ar_")
			return temp.substr(3);

		return name;
	}

	void start(char const* name)
	{
		if (indent > 0)
			std::cout << std::string(4*indent,' ');

		std::cout << "[" << clean_name(name) << "\n";

		++indent;
	}

	void end(char const* name)
	{
		assert(indent > 0 && "end() called once too many!");

		--indent;

		if (indent > 0)
			std::cout << std::string(4*indent,' ');

		std::cout << "]" << "\n";
	}

	void var(char const* name)
	{
		if (indent > 0)
			std::cout << std::string(4*indent,' ');
		std::cout << clean_name(name) << " = ";
	}

	template <class T>
	void archive(T& t)
	{
		std::cout << t << "\n";
	}

	size_t indent;
};

// typedef typename enable_if<has_member_XXX<T>::value,void>::type;

template<class T, class Archive>
typename enable_if<has_member_XXX<T>::value,void>::type archive_member_XXX(T& obj, Archive& arc)
{
	archive(obj.XXX, arc);
}

template<class T>
typename enable_if</**/!/**/has_member_XXX<T>::value,void>::type archive_member_XXX(T&, Archive& arc)
{
}

template<class T>
void _archive(T& obj, Archive& arc) // example
{
	archive_member_XXX(obj,arc);
}


#define HAS_MEMBER_VAR(XXX) \
template <class T> \
struct has_member_ ## XXX { \
	typedef char yes; \
	typedef long no; \
\
	struct Fallback { int XXX; }; \
	struct Derived : T, Fallback { }; \
\
	template <class C, C> struct ChT; \
\
	template <class C> static no  (&test(ChT<int Fallback::*, &C::XXX>*)); \
	template <class C> static yes (&test(...)); \
\
	enum { value = sizeof(test<Derived>(0)) == sizeof(yes) }; \
}

template<typename T>
void archive(T&, Archive& arc);

#define ARCHIVE_MEMBER(XXX) \
template<class T> \
typename enable_if<has_member_## XXX<T>::value,void>::type archive_member_## XXX(T& obj, Archive& arc)\
{\
	archive(#XXX, obj.XXX, arc);\
}\
\
template<class T>\
typename enable_if</**/!/**/has_member_## XXX<T>::value,void>::type archive_member_## XXX(T&, Archive& arc)\
{\
}

// ar_ — POOR MAN'S archivable ATTRIBUTE
// expand as needed
#define LIST X(ar_pos) X(ar_dir) X(ar_transform) X(ar_a) X(ar_b) X(ar_c) X(ar_x) X(ar_y) X(ar_z)

#define X(XXX) \
	HAS_MEMBER_VAR(XXX); \
	ARCHIVE_MEMBER(XXX)

LIST

#undef X


template<class T>
void archive(char const* name, T& obj, Archive& arc)
{
	arc.start(name);
#define X(XXX) archive_member_## XXX(obj,arc);
	LIST
#undef X
	arc.end(name);
}

void archive(char const* name, int& x, Archive& arc)
{
	arc.var(name);
	arc.archive(x);
}
void archive(char const* name, float& x, Archive& arc)
{
	arc.var(name);
	arc.archive(x);
}

struct A {
	int ar_a;
};

struct vec3 {
	float ar_x;
	float ar_y;
	float ar_z;
};

struct B {
	vec3 ar_pos;
};

struct C {
	float ar_a;
	vec3 ar_pos;
	vec3 ar_dir;
};

int main()
{
	/*
	std::cout << has_member_ar_pos<vec3>::value << "\n";
	std::cout << has_member_ar_c<vec3>::value << "\n";
	std::cout << has_member_ar_x<vec3>::value << "\n";
	std::cout << has_member_ar_y<vec3>::value << "\n";
	std::cout << has_member_ar_z<vec3>::value << "\n";*/

	Archive arc;
	A a; B b; C c; vec3 v;
	archive("a", a, arc);
	archive("b", b, arc);
	archive("c", c, arc);
	archive("v", v, arc);
}
