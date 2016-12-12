#include <string>

class Kata
{
	class iterator {
		size_t pos = 0;
		char c;
		std::string* sv;

	public:
		iterator(std::string& sv, char c) : sv{&sv}, c{c}
		{
			pos = sv.find(c, pos);
		}
		iterator() : pos{std::string::npos} {}
		char& operator*()
		{
			return (*sv)[pos];
		}

		iterator& operator++()
		{
			pos = sv->find(c, pos+1);
			return *this;
		}

		bool operator!=(iterator const& other)
		{
			return pos != other.pos;
		}
	};
public:
	std::string replaceNth(std::string text, int n, char oldValue, char newValue)
	{
		int i = 1;
		iterator it{text, oldValue};
		iterator end{};
		for (; it != end; ++it, ++i) {
			if (i % n == 0)
				*it = newValue;
		}
		return text;
	}
};

#include <iostream>
int main()
{
	std::string expected = "Vader soid: No, I am your fother!";

	Kata kata;
	std::string actual = kata.replaceNth("Vader said: No, I am your father!", 2, 'a', 'o');

	std::cout << expected << '\n';
	std::cout << actual << '\n';
}
