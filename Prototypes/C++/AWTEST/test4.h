#include <utility>
#include <functional>
#include <exception>
#include <vector>
#include <iostream>

namespace aw {
namespace test {
enum class stage : size_t {
	start,
	setup,
	preconditions,
	check,
	postconditions,
	end
};

// TODO: C++17: inline
static char const* stage_name[] {
	"start",
	"setup",
	"preconditions",
	"check",
	"postconditions",
	"end"
};

struct test_failed : std::exception {};

namespace {
struct test {
	using test_function = void();
	test(char const* name);

	char const*    const name;

private:
	friend struct context;

	virtual void setup() = 0;
	virtual void preconditions() = 0;
	virtual void checks() = 0;
	virtual void postconditions() = 0;

	stage st = stage::start;

	size_t failed   = 0;
	size_t succeded = 0;
};

struct context {
	context(char const* filename)
		: filename(filename)
	{}

	test& current() { return *cur; }
	char const* const filename;

	int run()
	{
		std::cerr << "running tests for " << filename << '\n';
		for (size_t i = 0, e = tests.size(); i < e; ++i)
			run(tests[i].get(), i);

		std::cerr << "tests for " << filename << " are complete, failed: ";
		std::cerr << failed << '\n';

		return failed > 0xFF ? 0xFF : failed;
	}

private:
	friend class context_check;
	friend class context_test;

	void run(test& tst, size_t i);


	void enter(stage st)
	{
		if (cur->failed > 0)
			throw test_failed{};
		cur->st = st;
	}

	void leave()
	{
	}

	void add_test(test& tst)
	{
		tests.push_back(std::ref(tst));
	}

	void check_fail()
	{
		++cur->failed;
	}

	void check_succeed()
	{
		++cur->succeded;
	}

private:
	test* cur;
	std::vector<std::reference_wrapper<test>> tests;
	unsigned failed = 0;
};

void context::run(test& tst, size_t i)
{
	size_t e = tests.size();
	cur = &tst;
	std::cerr << '[' << i+1 << '/' << e << "] " << "test " << cur->name;


	try {
		cur->setup();
		if (failed) throw test_failed{};
		cur->preconditions();
		if (failed) throw test_failed{};
		cur->checks();
		if (failed) throw test_failed{};
		cur->postconditions();
		if (failed) throw test_failed{};

		std::cerr << " succeded: checks: " << cur->succeded << '\n';
	} catch (test_failed& ex) {
		++failed;
		std::cerr << " failed: (" << stage_name[size_t(cur->st)] << ") ";
		std::cerr << "failed checks: " << cur->failed << '\n';
	}
}

extern context& file_context;

class context_check {
	template<template <typename...> class Checker, typename... Args>
	friend void check(Args&&... args);

	static void check_fail()
	{
		file_context.check_fail();
	}
	static void check_succeed()
	{
		file_context.check_succeed();
	}
};

class context_test {
	friend struct test;
	static void add_test(test& tst)
	{
		file_context.add_test(tst);
	}
};

test::test(char const* name)
	: name{name}
{
	context_test::add_test(*this);
}

template<template <typename...> class Checker, typename... Args>
void check(Args&&... args)
{
	Checker<Args...> check;
	check(std::forward<Args>(args)...) ?
		context_check::check_succeed() :
		context_check::check_fail();
}
} // namespace


template<typename A, typename B>
struct equal {
	bool operator()(A const& a, B const& b)
	{
		return a == b;
	}
};
} // namespace test
} // namespace aw


namespace aw {
namespace test {
template<char const* name, class T>
struct test_name : test {
	test_name() : test(name) {}

	static T t;
};

template<char const* name, class T>
T test_name<name,T>::t = T{};
} // namespace test
} // namespace aw


// 
#define TestFile(name) namespace aw { namespace test { namespace { context  ctx{ name }; context& file_context{ctx}; } } }
#define Test(name)     constexpr char name##_name[] = #name; struct test_##name : aw::test::test_name<name##_name, test_##name>
#define Setup          void setup() override
#define Preconditions  void preconditions() override
#define Checks         void checks() override
#define Postconditions void postconditions() override
#define RunTests       aw::test::file_context.run
