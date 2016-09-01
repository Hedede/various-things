#include <set>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <aw/utility/algorithm.h>

int main()
{
	std::set<std::string> commands;

	commands.insert("set texture quality");
	commands.insert("set text");
	commands.insert("set time");
	commands.insert("set gravity");
	commands.insert("test audio");
	commands.insert("test video");

	std::vector<std::string> history;

	std::string str;
	while ( std::getline(std::cin, str, '\n') ) {
		auto pos = std::remove( begin(history), end(history), str );
		history.erase(pos, end(history));
		history.push_back(str);

		std::cout << "-------------------\n";
		for (auto const& s : history)
			std::cout << ">" << s << "\n";

		std::vector<std::string> matches;
		auto it = commands.lower_bound(str);

		while ( (it != end(commands)) && (it->find(str) == 0) )
			matches.push_back(*it++);

		if (matches.empty())
			continue;

		if (matches.size() == 1) {
			if (matches[0] != str)
				std::cout << "hint: " << matches[0] << "\n";
			continue;
		}

		size_t prefix = aw::common_prefix_length(
			begin(matches), end(matches), str.size());

		if (prefix == str.size())
			continue;

		std::cout << "hint: " << matches[0].substr(0, prefix) << "\n";
	}
}
