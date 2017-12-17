#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <sstream>
#include <limits>
#include <tuple>

template<typename T>
decltype(auto) operator--(T&& tuple)
{
	auto decrement = [] (auto&&... a) {
		(--std::forward<decltype(a)>(a), ... );
	};
	return std::apply( decrement, std::forward<T>(tuple) );
}

int main()
{
	// i skipped all checking for input though
	unsigned num_rows, num_cols, num_tracks;
	std::cin >> num_rows >> num_cols >> num_tracks;
	//skip rest of the bull ship
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	using track = std::pair<int,int>;
	using row = std::vector<track>;
	std::vector<row> rows( num_rows );

	std::string line;
	std::stringstream ss;
	while (std::getline(std::cin,line)) {
		unsigned row, start, end;
		ss.str(line);
		ss >> row >> start >> end;
		--std::tie(row,start,end);
		rows[row].emplace_back(start,end);
	}

	size_t free = 0;
	for (auto& row:rows) {
		std::sort(row.begin(),row.end(),[] (auto a, auto b) { return a.first < b.first; });
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

	std::cout << free << '\n';

}
