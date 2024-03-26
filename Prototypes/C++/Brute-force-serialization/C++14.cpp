#include <utility>
#include <type_traits>
/* void_t */
template<typename... Ts> struct make_void { using type = void; };
template<typename... Ts> using void_t = typename make_void<Ts...>::type;

/* void_if */
template <bool expr> using void_if = std::enable_if_t<expr, void>;

/* detector */
namespace detail {
template <class, template<class...> class Op, class... Args>
struct detector : std::false_type {};

template <template<class...> class Op, class... Args>
struct detector<void_t<Op<Args...>>, Op, Args...> : std::true_type {};
} // namespace detail

template <template<class...> class Op, class... Args>
constexpr auto is_detected = detail::detector<void, Op, Args...>::value;

/** Archive **/
#include <cassert>
#include <cstring>
#include <string>
#include <iostream>

struct Archive {
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

	size_t indent = 0;
};

template<class T> using member_XXX = decltype(std::declval<T>().XXX);

template<class T, class Archive>
auto archive_member_XXX(T& obj, Archive& arc) -> void_if<is_detected<member_XXX,T>>
{
	archive(obj.XXX, arc);
}

template<class T>
auto archive_member_XXX(T&, Archive& arc) -> void_if<!is_detected<member_XXX,T>> { }

template<class T>
void _archive(T& obj, Archive& arc) // example
{
	archive_member_XXX(obj,arc);
}

#define HAS_MEMBER_VAR(XXX) \
template<class T> using member_## XXX = decltype(std::declval<T>().XXX); \
template<class T> constexpr bool has_member_## XXX = is_detected<member_## XXX,T>;

#define ARCHIVE_MEMBER(XXX) \
template<class T> \
auto archive_member_## XXX(T& obj, Archive& arc) -> void_if<has_member_ ## XXX<T>> \
{\
	archive(#XXX, obj.XXX, arc);\
}\
\
template<class T>\
auto archive_member_## XXX(T&, Archive& arc) -> void_if<!has_member_ ## XXX<T>> { }

// ar_ — POOR MAN'S archivable ATTRIBUTE
#define ARCHIVABLE(x) ar_##x
// expand as needed
#define LIST X(ar_pos) X(ar_dir) X(ar_transform) X(ar_rotation) X(ar_state) X(ar_a) X(ar_b) X(ar_c) X(ar_d) X(ar_e) X(ar_f) X(ar_g) X(ar_h) X(ar_i) X(ar_j) X(ar_k) X(ar_l) X(ar_m) X(ar_n) X(ar_o) X(ar_p) X(ar_q) X(ar_r) X(ar_s) X(ar_t) X(ar_u) X(ar_w) X(ar_v) X(ar_x) X(ar_y) X(ar_z)

#define X(XXX) HAS_MEMBER_VAR(XXX); ARCHIVE_MEMBER(XXX)
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
	int ARCHIVABLE(a);
};

struct vec3 {
	float ARCHIVABLE(x);
	float ARCHIVABLE(y);
	float ARCHIVABLE(z);
};

struct B {
	vec3 ARCHIVABLE(pos);
};

struct C {
	float ARCHIVABLE(a);
	vec3  ARCHIVABLE(pos);
	vec3  ARCHIVABLE(dir);
};

int main()
{
	Archive arc;
	A a {1000};
	B b {
		{10.0,23.5,44.0}
	};
	C c {
		15.0,
		{0.0,0.0,0.0},
		{1.0,0.0,0.0}
	};
	vec3 v{0.7423,0.2144,1 - 0.7423 - 0.2144};

	archive("a", a, arc);
	archive("b", b, arc);
	archive("c", c, arc);
	archive("v", v, arc);
}
