#include <iostream>
#include <vector>
std::vector<int> longest_increasing_subsequence( std::vector<int> const& input );

std::ostream& operator<<( std::ostream& os, std::vector<int> const& a )
{
	os << '{';
	if (a.size() > 0)
		os << a[0];
	for (int i = 1; i < a.size(); ++i)
		os << ", " << a[i];
	os << '}';
	return os;
}

template<typename T>
static bool check(T const& a, decltype(a) b)
{
	if (a == b)
		return true;
	std::cerr << "expect: " << b << '\n';
	std::cerr << "result: " << a << '\n';
	return false;
}

/* solver tests */
bool test_empty()
{
	std::vector<int> empty;
	auto result = longest_increasing_subsequence( empty );
	return check(result, empty);
}

bool test_single()
{
	std::vector<int> expect = { 8 };
	auto result = longest_increasing_subsequence( expect );
	return check(result, expect);
}

bool test_sorted()
{
	std::vector<int> expect = { 1, 2, 5, 8, 10, 11, 24, 25, 30 };
	auto result = longest_increasing_subsequence( expect );
	return check(result, expect);
}

bool test_reverse()
{
	std::vector<int> input = { 8, 6, 5, 4, 3, 1 };
	std::vector<int> expect = { 1 };
	auto result = longest_increasing_subsequence( input );
	return check(result, expect);
}

bool test_all_same()
{
	std::vector<int> input = { 8, 8, 8, 8, 8, 8 };
	std::vector<int> expect = { 8 };
	auto result = longest_increasing_subsequence( input );
	return check(result, expect);
}

bool test_short()
{
	std::vector<int> input = { 6, 1, 2, 3, 4, 2 };
	std::vector<int> expect = { 1, 2, 3, 4 };
	auto result = longest_increasing_subsequence( input );
	return check(result, expect);
}

bool test_longer()
{
	std::vector<int> input = { 1, 2, 10, 3, 4, 11, 8, 12, 15, 7, 3, 25, 16 };
	std::vector<int> expect = { 1, 2, 3, 4, 8, 12, 15, 16 };
	auto result = longest_increasing_subsequence( input );
	return check(result, expect);
}

struct test {
	bool(*func)();
	const char* name;
};

test tests[] = {
	{test_empty,    "test_empty"},
	{test_single,   "test_single"},
	{test_all_same, "test_all_same"},
	{test_sorted,   "test_sorted"},
	{test_reverse,  "test_reverse"},
	{test_short,    "test_short"},
	{test_longer,   "test_longer"},
};

int main()
{
	int num_failed = 0;
	for (auto test : tests)
	{
		if(!test.func()) {
			std::cerr << "--Test '" << test.name << "' failed!\n";
			++num_failed;
		}
	}

	if (num_failed > 0) {
		std::cerr << "All tests have finished. Failed: ";
		std::cerr << num_failed << '/' << std::size(tests) << '\n';
	} else {
		std::cerr << "All tests have succeeded [";
		std::cerr << std::size(tests) << '/' << std::size(tests) << "].\n";
	}
}
