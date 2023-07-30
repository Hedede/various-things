#include <iostream>
#include <vector>
#include <map>

using milliseconds = unsigned long long;
struct trade {
	milliseconds time;
	double price;
	unsigned long size;
};


inline std::vector<std::string> cut(std::string const& s, char delim)
{
	std::vector<std::string> holder;
	size_t pos1 = 0;
	size_t pos2;

	do {
		pos2 = s.find(delim, pos1);

		holder.push_back( s.substr(pos1, pos2 - pos1) );

		if (pos2 == s.npos)
			break;

		pos1 = pos2 + 1;
	} while (true);

	return holder;
}

constexpr std::uint64_t hour_to_ms = 3600 * 1000;
constexpr std::uint64_t minute_to_ms = 60 * 1000;
constexpr std::uint64_t second_to_ms = 1000;
milliseconds parse_timestamp(std::string const& s)
{

	auto hms = cut(s,':');
	auto h = std::stoll(hms[0]);
	auto m = std::stoll(hms[1]);

	auto sec_ms = cut(hms[2],'.');

	auto sec = std::stoll(sec_ms[0]);
	auto ms = std::stoll(sec_ms[1]);
	return h*hour_to_ms + m*minute_to_ms + sec*second_to_ms + ms;
}

using iterator = std::vector<trade>::const_iterator;

iterator advance_1_minute(iterator begin, iterator end, size_t start)
{
	size_t count = 0;
	for (; begin != end; ++begin)
	{
		auto& trade = *begin;
		auto time_diff = trade.time - start;
		if ( time_diff >= minute_to_ms )
			break;
		++count;
	}
	return begin;
}

size_t max_window(std::vector<trade> const& v)
{
	auto max = 0;

	for (auto it = v.begin(), end = v.end(); std::distance(it,end) > max; ++it)
	{
		auto window_end = it + max;

		auto& t1 = *it;
		auto& t2 = *window_end;

		window_end = advance_1_minute(window_end, end, it->time);

		auto distance = std::distance(it,window_end);
		if (distance > max)
			max = distance;
	}

	return max;
}

int main()
{
	std::map<std::string, std::vector<trade>> exchanges;
	std::string line;
	while (std::getline(std::cin,line))
	{
		auto v = cut(line,',');
		// assume no incorrect input
		trade t {
			parse_timestamp(v[0]),
			std::stod(v[1]),
			std::stoul(v[2]),
		};
		auto& exchange = v[3];
		exchanges[exchange].push_back(t);
	}

	for (auto& e : exchanges)
		std::cout << max_window(e.second) << '\n';
}
