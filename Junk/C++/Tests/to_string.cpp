#include <string>
namespace aw {
std::string to_string(char const* s)
{
	return s;
}

std::string to_string(char c)
{
	return std::string(1,c);
}

std::string to_string(int i)
{
	return to_string(char('0' + i % 10));
}
}

#include <utility>

namespace aw {
template<class A, class B>
std::string to_string(std::pair<A,B> const& p)
{
	using aw::to_string;
	return to_string(p.first) + ", " + to_string(p.second);
}
}

#include <vector>
namespace aw {
template<class T>
std::string to_string(std::vector<T> const& p)
{
	using aw::to_string;
	std::string result;
	for (auto& v : p)
		result += to_string(v);
	return result;
}
}

#include <iostream>
namespace aw {
void main()
{
	std::cout << to_string("hren") << '\n';
	std::cout << to_string('c') << '\n';
	std::cout << to_string(19) << '\n';
	std::cout << to_string(std::make_pair(10,20)) << '\n';
	std::vector<int> v1;
	v1.push_back(10);
	v1.push_back(11);
	v1.push_back(12);
	v1.push_back(13);
	std::cout << to_string(v1) << '\n';
	// error v
	std::cout << to_string(std::make_pair(v1,v1)) << '\n';
}
}

int main()
{
	aw::main();
}