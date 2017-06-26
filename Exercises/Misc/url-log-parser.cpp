#include <set>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>


using namespace std::string_literals;

/* helpers */
template<typename T>
void print(T const& s)
{
	std::cout << s << '\n';
}

template<typename T>
bool in(T const& t, T const& a, T const& b)
{
	return t == a || t == b;
}

template<typename Criteria>
bool validate(std::string const& str, Criteria criteria)
{
	return std::find_if_not(begin(str), end(str), criteria) == end(str);
}

bool validate_domain(std::string const& url)
{
	auto criteria = [] (char c) {
		return std::isalnum(c) || ".-"s.find(c) != std::string::npos;
	};
	return validate(url, criteria);
}

auto valid_path_char = [] (char c) {
	return std::isalnum(c)  || ".,/+_"s.find(c) != std::string::npos;
};
bool validate_path(std::string const& url)
{
	return validate(url, valid_path_char);
}

// this is an one-off thing, globals will do
namespace {
int total_urls = 0;
struct domain {
	std::set<std::string> paths;
	int uses = 0;
};
std::map<std::string, domain> urls;
}

/* actual parser */

void parse_url(std::string const& str)
{
	auto pos = str.find('/');
	auto domain = str.substr(0, pos);
	if (!validate_domain(domain))
		return;
	std::string path{"/"};
	if (pos != str.npos) {
		path = str.substr(pos);
		if (!validate_path(path))
			return;
	}
	urls[domain].paths.insert(path);
	++urls[domain].uses;
	++total_urls;
}

template<typename Func>
std::string::size_type find_if_not(std::string const& str, Func condition, size_t pos)
{
	auto _begin = begin(str) + pos;
	auto it = std::find_if_not(_begin, end(str), condition);
	if (it == end(str))
		return str.npos;
	return it - begin(str);
}

void parse_line(std::string const& line)
{
	auto pos = line.find("http");
	auto pos2 = line.find("//", pos);

	while (pos != line.npos && pos2 != line.npos) {
		if (!in(line.substr(pos, pos2-pos), "http:"s, "https:"s)) {
			pos = pos2;
			continue;
		}

		pos2 += 2;

		pos = find_if_not(line, valid_path_char, pos2);

		parse_url(line.substr(pos2,pos-pos2));

		pos = line.find("http", pos2);
		pos2 = line.find("//", pos);
	}
}

void parse(int n, std::istream& in, std::ostream& out)
{
	std::string line;
	while (std::getline(in, line))
		parse_line(line);

	std::vector<std::pair<int, std::string>> domains;
	std::map<std::string, int> paths;
	for (auto& p : urls) {
		domains.emplace_back(p.second.uses, p.first);
		for (auto& path : p.second.paths)
			++paths[path];
	}
	auto compare = [] (auto pair1, auto pair2) {
		return pair1.first >  pair2.first ||
		       pair1.first == pair2.first && pair1.second < pair2.second;
	};
	std::sort(begin(domains), end(domains), compare);

	out << "total urls " << total_urls << ", domains " << domains.size() << ", paths " << paths.size() << '\n';
	out << "top domains\n";

	int i = 0;
	for (auto domain : domains) {
		if (i++ > n)
			break;
		out << domain.first << ' ' << domain.second << '\n';
	}

	out << "top paths\n";
	std::vector<std::pair<int, std::string>> paths_vec;
	for (auto& p : paths)
		paths_vec.emplace_back(p.second, p.first);

	std::sort(begin(paths_vec), end(paths_vec), compare);

	i = 0;
	for (auto path : paths_vec) {
		if (i++ > n)
			break;
		out << path.first << ' ' << path.second << '\n';
	}
}


int main(int, char** args)
{
	int n = std::numeric_limits<int>::max();
	char* arg = *++args;
	if (arg) {
		if (arg == "-n"s) {
			arg = *++args;
			if (!arg) {
				std::cerr << "Missing parameter to -n.\n";
				return EXIT_FAILURE;
			}

			char* end;
			n = std::strtol(arg, &end, 10);
			if (end == arg || n < 0) {
				std::cerr << "Invalid parameter to -n.\n";
				return EXIT_FAILURE;
			}

			arg = *++args;
		}
	}

	std::ifstream ifile;
	std::ofstream ofile;

	if (arg) {
		ifile.open(arg);
		arg = *++args;
	}
	if (arg) {
		ofile.open(arg);
		arg = *++args;
	}

	parse(n, ifile.is_open()? ifile : std::cin, ofile.is_open()? ofile : std::cout);
}
