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



struct ArchiveBase {
};

template <typename Derived>
struct Archive : ArchiveBase {
	Derived& derived;

	OutputArchive(Derived& derived)
		: derived(derived)
	{
	}

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

