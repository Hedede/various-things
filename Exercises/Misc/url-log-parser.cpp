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

auto valid_domain_char = [] (char c) {
	return std::isalnum(c) || ".-"s.find(c) != std::string::npos;
};

auto valid_path_char = [] (char c) {
	return std::isalnum(c) || ".,/+_"s.find(c) != std::string::npos;
};

// this is an one-off thing, globals will do
namespace {
int total_urls;
std::map<std::string, int> domains;
std::map<std::string, int> paths;
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

size_t find_protocol(std::string const& line, size_t start = 0)
{
	constexpr char http[] = "http";
	constexpr char http_suf[] = "://";
	constexpr char https_suf[] = "s://";

	auto pos = line.find(http, start);
	while (pos != line.npos) {
		pos += sizeof(http)-1;

		if (line.substr( pos, sizeof(http_suf)-1 ) == http_suf )
			return pos + sizeof(http_suf)-1;
		if (line.substr( pos, sizeof(https_suf)-1 ) == https_suf )
			return pos + sizeof(https_suf)-1;

		pos = line.find(http, pos);
	}
	return pos;
}

size_t parse_url(std::string const& line, size_t pos)
{
	size_t domain_end = find_if_not(line, valid_domain_char, pos);

	if (pos == domain_end)
		return pos;
	++total_urls;

	auto domain = line.substr(pos, domain_end - pos);
	++domains[domain];

	if (domain_end == line.npos || line[domain_end] != '/') {
		++paths["/"];
		return domain_end;
	}

	size_t path_end = find_if_not(line, valid_path_char, domain_end);
	auto path = line.substr(domain_end, path_end - domain_end);
	++paths[path];

	return path_end;
}

void parse_line(std::string const& line)
{
	auto pos = find_protocol(line);
	while (pos < line.size()) {
		auto pos2 = parse_url(line, pos);
		pos = find_protocol(line, pos2);
	}
}

void print_result(std::ostream& out, std::map<std::string, int> const& map, int max)
{
	std::vector<std::pair<std::string, int>> vec{begin(map), end(map)};
	auto compare = [] (auto const& a, auto const& b) {
		return a.second > b.second;
	};

	std::stable_sort(begin(vec), end(vec), compare);

	int i = 0;
	for (auto const& str : vec) {
		if (i++ > max)
			break;
		out << '\t' << str.second << ' ' << str.first << '\n';
	}
}

void parse(int n, std::istream& in, std::ostream& out)
{
	std::string line;
	while (std::getline(in, line))
		parse_line(line);

	out << "total urls " << total_urls << ", domains " << domains.size() << ", paths " << paths.size() << '\n';
	out << "\ntop domains\n";
	print_result( out, domains, n );
	out << "\ntop paths\n";
	print_result( out, paths, n );
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
