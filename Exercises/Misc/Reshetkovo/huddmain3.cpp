#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <sstream>
#include <limits>
#include <tuple>
#include <stdexcept>

template<typename T>
decltype(auto) operator--(T&& tuple)
{
	auto decrement = [] (auto&&... a) {
		(--std::forward<decltype(a)>(a), ... );
	};
	return std::apply( decrement, std::forward<T>(tuple) );
}

// skip rest of the line
std::istream& skip_endl(std::istream& is)
{
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	return is;
}

template<typename T>
bool within(T const& value, T const& min, T const& max)
{
	return value >= min && value <= max;
}

struct city_planner {
	static constexpr auto min_dimen = 1u;
	static constexpr auto max_dimen = 1'000'000'000u;
	static constexpr auto min_tracks = 0u;
	static constexpr auto max_tracks = 1000u;

	using track = std::pair<int,int>;
	using row   = std::vector<track>;

	city_planner(std::istream& is);

	unsigned free_cells() const;

private:
	void check_grid_params(unsigned n, unsigned m, unsigned k);
	void check_track_params(unsigned r, unsigned c1, unsigned c2);
	void read_rows(std::istream& is);
	void sort_rows();

	unsigned num_rows, num_cols, num_tracks;
	unsigned free = 0;

	std::vector<row> rows;
};

void city_planner::check_grid_params(unsigned n, unsigned m, unsigned k)
{
	if (!within(n, min_dimen, max_dimen))
		throw std::runtime_error{"n must be within 1 and 10^9"};
	if (!within(m, min_dimen, max_dimen))
		throw std::runtime_error{"m must be within 1 and 10^9"};
	if (!within(k, min_tracks, max_tracks))
		throw std::runtime_error{"k must be within 0 and 1000"};
}

void city_planner::check_track_params(unsigned r, unsigned c1, unsigned c2)
{
	if (!within(r, 1u, num_rows))
		throw std::runtime_error{"r must be between 1 and n"};
	if (!within(c1, 1u, num_cols))
		throw std::runtime_error{"c1 must be between 1 and m"};
	if (!within(c2, 1u, num_cols))
		throw std::runtime_error{"c1 must be between 1 and m"};
}

city_planner::city_planner(std::istream& is)
{
	is >> num_rows >> num_cols >> num_tracks >> skip_endl;
	check_grid_params(num_rows, num_cols, num_tracks);
	rows.resize(num_rows);

	read_rows(is);
	sort_rows();
}

void city_planner::read_rows(std::istream& is)
{
	unsigned cur_track = 0;
	while (std::cin) {
		if (++cur_track > num_tracks) {
			std::cerr << "warning: data contains more tracks than specified\n";
			break;
		}

		unsigned row, start, end;
		is >> row >> start >> end >> skip_endl;
		check_track_params(row, start, end);

		if (start > end)
			std::swap(start,end);

		--std::tie(row,start,end);
		rows[row].emplace_back(start,end);
	}
}

void city_planner::sort_rows()
{
	for (auto& row:rows) {
		auto cmp = [] (auto a, auto b) { return a.first < b.first; };
		std::sort(row.begin(),row.end(), cmp);
	}
}

unsigned city_planner::free_cells() const
{
	size_t free = 0;
	for (auto& row:rows) {
		int cur_start = 0;
		int prev_end  = 0;
		for (auto [track_start,track_end]:row) {
			cur_start = track_start;
			if (int d = cur_start - prev_end; d > 0)
				free += d;
			prev_end = track_end;
		}
		free += (num_cols-1) - prev_end;
	}

	return free;
}

int main()
{
	try {
		city_planner planner{std::cin};
		std::cout << planner.free_cells() << '\n';
	} catch(std::exception& ex) {
		std::cerr << ex.what() << '\n';
	}
}
