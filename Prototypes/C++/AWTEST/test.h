#ifndef aw_test_test_h
#define aw_test_test_h
#include <utility>
#include <functional>
#include <exception>
#include <vector>
#include <iostream>

/*!
 * This header is made for awlib internal tests.
 * It is so simple and crude, that it doesn't even
 * deserve separate directory.
 */

namespace aw {
char const bold[]  = "\033[1m";
char const red[]   = "\033[31m";
char const green[] = "\033[32m";
char const white[] = "\033[37m";
char const reset[] = "\033[0m";

namespace test {
enum class stage : size_t {
	start,
	setup,
	preconditions,
	checks,
	postconditions,
	end
};

static char const* stage_name[] {
	"start",
	"setup",
	"preconditions",
	"checks",
	"postconditions",
	"end"
};

struct test {
	using test_function = void();
	test(char const* name, test_function* func)
		: name{name}, func{func}
	{ }

	char const*    const name;

private:
	friend struct context;

	test_function* const func;

	stage st = stage::start;

	size_t failed   = 0;
	size_t succeded = 0;
};

struct test_failed : std::exception {};

template<typename... Args>
void print(Args&&...args)
{
	( std::cerr << ... << std::forward<Args>(args) );
}

namespace {
class context_check;
class context_block;
class register_test;
}

struct context {
	context(char const* filename)
		: filename(filename)
	{}

	test& current() { return *cur; }
	char const* const filename;

	int run()
	{
		print(bold, '[', filename, ']', ' ', reset);
		print("running tests\n");

		for (size_t i = 0, e = tests.size(); i < e; ++i) {
			print(bold, '[', i+1, '/', e, ']', ' ', reset);
			run_test(tests[i]);
		}

		print(bold, '[', filename, ']', ' ', reset);
		print("tests done, failed: ");
		print(bold, (failed > 0 ? red : white), failed, reset);
		print(", succeeded: ");
		size_t succeded = tests.size() - failed;
		print(bold, (succeded > 0 ? green : white), succeded, reset);
		print(reset, '\n');

		return failed > 0xFF ? 0xFF : failed;
	}

private:
	friend class aw::test::context_check;
	friend class aw::test::context_block;
	friend class aw::test::register_test;

	void run_test(test& tst);

	void enter(stage st)
	{
		if (cur->failed > 0)
			throw test_failed{};
		cur->st = st;
	}

	void leave()
	{
	}

	void add_test(test&& tst)
	{
		tests.push_back(std::move(tst));
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
	std::vector<test> tests;
	unsigned failed = 0;
};

void context::run_test(test& tst)
{
	cur = &tst;
	print("test \"", bold, cur->name, reset, '"');
	try {
		enter(stage::start);
		cur->func();
		enter(stage::end);
		print(bold, green, " succeded, checks: ", cur->succeded, reset, '\n');
	} catch (test_failed& ex) {
		++failed;
		print(bold, red, " failed: (", stage_name[size_t(cur->st)], ") ");
		std::cerr << "failed checks: " << cur->failed << reset << '\n';
	}
}

namespace {
extern context file_context;

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

class context_block {
	friend bool setup();
	friend bool preconditions();
	friend bool checks();
	friend bool postconditions();

	static void enter(stage st)
	{
		file_context.enter(st);
	}
	static void leave()
	{
		file_context.leave();
	}
};

class register_test {
protected:
	void add_test(test&& tst)
	{
		file_context.add_test(std::move(tst));
	}
};

bool setup()          { context_block::enter(stage::setup); return true; }
bool preconditions()  { context_block::enter(stage::preconditions); return true; }
bool checks()         { context_block::enter(stage::checks); return true; }
bool postconditions() { context_block::enter(stage::postconditions); return true; }

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

#define TestFile(name) namespace aw { namespace test { namespace { context file_context{name}; } } }
#define Test(name)     \
	void run_test_##name(); \
	struct Add_test_##name : aw::test::register_test { \
		Add_test_##name() { \
			add_test(aw::test::test{#name, run_test_##name}); \
		} \
	} add_test_##name; \
	void run_test_##name()
#define Setup          if (aw::test::setup())
#define Preconditions  if (aw::test::preconditions())
#define Checks         if (aw::test::checks())
#define Postconditions if (aw::test::postconditions())
#define RunTests       aw::test::file_context.run

#endif//aw_test_test_h
