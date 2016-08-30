#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

int main()
{
	std::vector<std::string> history;

	std::string str;
	while ( std::getline(std::cin, str, '\n') ) {
		auto pos = std::remove( begin(history), end(history), str );
		history.erase(pos, end(history));
		history.push_back(str);

		std::cout << "-------------------\n";
		for (auto const& s : history)
			std::cout << ">" << s << "\n";
	}
}
