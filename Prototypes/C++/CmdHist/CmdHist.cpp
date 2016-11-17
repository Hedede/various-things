#include <set>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <aw/utility/algorithm.h>

struct Compare {
	bool operator() (std::string const& a, std::string const& b) {
		size_t const N = std::min(a.size(), b.size());
		return a.substr(0,N) < b.substr(0, N);
	}
} compare;

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

	auto find = [&] (std::string const& cmd) {
	
		unsigned i = 0;

		auto pos = cmd.find(' ');
		auto prefix = cmd.substr(0, pos);

		auto r = std::equal_range(commands.begin(), commands.end(), prefix, compare);
		auto r2 = r;
		while (r2.first != r.second) {
			++i;
			if (pos == std::string::npos)
				break;
			pos = cmd.find(' ', pos+1);
			prefix = cmd.substr(0, pos);
			r = r2;
			r2 = std::equal_range(r.first, r.second, prefix, compare);
		}

		std::cout << i << '\n';

		if (std::distance(r2.first, r2.second) != 1)
			return commands.end();

		if (r2.first != commands.end())
			if (*r2.first != prefix)
				return commands.end();

		return r2.first;
	};

	std::string str;
	while ( std::getline(std::cin, str, '\n') ) {
		auto pos = std::remove( begin(history), end(history), str );
		history.erase(pos, end(history));
		history.push_back(str);

		std::cout << "-------------------\n";
		for (auto const& s : history)
			std::cout << "^" << s << "\n";

		std::cout << ">" << str << '\n';
		auto cmd = find(str);
		if (cmd != commands.end())
			std::cout << "!" << *cmd << "\n";

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
