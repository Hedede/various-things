#include <utility>
#include <functional>
#include <exception>
#include <vector>
#include <iostream>

namespace test {
enum class stage : size_t {
	start,
	setup,
	preconditions,
	check,
	postconditions,
	end
};

char const* stage_name[] {
	"start",
	"setup",
	"preconditions",
	"check",
	"postconditions",
	"end"
};

struct test {
	using test_function = void();
	test(char const* name, test_function* func);

	char const*    const name;

private:
	friend struct context;

	test_function* const func;

	stage st = stage::start;

	size_t failed   = 0;
	size_t succeded = 0;
};

struct block {
	block(stage st);
	~block();
};

struct test_failed : std::exception {};

struct context_access {
	class check;
	class block;
	class test;
};

struct context {
	context(char const* filename)
		: filename(filename)
	{}

	test& current() { return *cur; }
	char const* const filename;

	int run()
	{
		for (size_t i = 0, e = tests.size(); i < e; ++i) {
			try {
				cur = &tests[i].get();
				cur->func();
				block _end_ {stage::end};
				std::cerr << '[' << i+1 << '/' << e << "] " << "test " << cur->name;
				std::cerr << " succeded: checks: " << cur->succeded << '\n';
			} catch (test_failed& ex) {
				++failed;
				std::cerr << '[' << i+1 << '/' << e << "] " << "test " << cur->name;
				std::cerr << " failed: (" << stage_name[size_t(cur->st)] << ") ";
				std::cerr << "failed checks: " << cur->failed << '\n';
			}
		}

		return failed > 0xFF ? 0xFF : failed;
	}

private:
	friend class context_access::check;
	friend class context_access::block;
	friend class context_access::test;

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

namespace {
extern context& file_context;
}

template<template <typename...> class Checker, typename... Args>
void check(Args&&... args);
class context_access::check {
	template<template <typename...> class Checker, typename... Args>
	friend void ::test::check(Args&&... args);

	static void check_fail()
	{
		file_context.check_fail();
	}
	static void check_succeed()
	{
		file_context.check_succeed();
	}
};

class context_access::block {
	friend struct ::test::block;
	static void enter(stage st)
	{
		file_context.enter(st);
	}
	static void leave()
	{
		file_context.leave();
	}
};

class context_access::test {
	friend struct ::test::test;
	static void add_test(::test::test& tst)
	{
		file_context.add_test(tst);
	}
};

block::block(stage st) { context_access::block::enter(st); }
block::~block()        { context_access::block::leave(); }


test::test(char const* name, test_function* func)
	: name{name}, func{func}
{
	context_access::test::add_test(*this);
}

template<typename A, typename B>
struct equal {
	bool operator()(A const& a, B const& b)
	{
		return a == b;
	}
};

template<template <typename...> class Checker, typename... Args>
void check(Args&&... args)
{
	Checker<Args...> check;
	check(std::forward<Args>(args)...) ?
		context_access::check::check_succeed() :
		context_access::check::check_fail();
}
} // namespace test

namespace test {
namespace {
context  ctx{"FILENAME"};
context& file_context{ctx};
}
}


struct stuff {
	void do_that() {}
	void do_this() {}
	bool empty() { return false; }

	int whatever = 0;
} something;

test::test sometest{
	"sometest",
	[] {
		stuff x;

		{test::block _1_ {test::stage::setup};
			x = something;
		}

		{test::block _2_ {test::stage::preconditions};
			int value = 0;
			test::check<test::equal>(x.whatever, value);
		}

		{test::block _3_ {test::stage::check};
			x.do_this();
			x.do_that();
		}

		{test::block _4_ {test::stage::postconditions};
			test::check<test::equal>(x.empty(), true);
		}
	}
};

int main()
{
	return test::file_context.run();
}
