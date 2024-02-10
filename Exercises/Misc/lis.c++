#include <vector>
#include <limits>
#include <cassert>

std::vector<int> longest_increasing_subsequence( const std::vector<int>& input )
{
	if ( input.size() <= 0 )
		return {};

	// contains the last value of the LIS of size n
	// (n == index of the cell)
	std::vector<int> last( input.size() + 1 );
	last.fill( 0 );

	// stores the length of the LIS for the array
	// up to nth element
	std::vector<int> length( input.size() );

	// longest LIS found so far
	int max_lis = 0;

	// binary search for position of the element in array 'last'
	auto find_pos = [&] (int i) {
		auto left = 1;
		auto right = max_lis;
		while (left <= right) {
			auto mid = (left+right)/2;
			if (input[last[mid]] < input[i])
				left  = mid + 1;
			else
				right = mid - 1;
		}
		return left;
	};

	// find size of the LIS for each element
	for (int i = 0; i < input.size(); ++i) {
		auto pos = find_pos(i);
		last[pos] = i;
		length[i] = pos;
		if (pos > max_lis)
			max_lis = pos;
	}

	// reconstruct the LIS by scanning the array backwards,
	// picking elements such that:
	// 1. LISₙ₋₁ is smaller than LISₙ
	// 2. Lₙ is equal to n
	std::vector<int> lis( max_lis );

	// find the index for last element in the LIS
	int j = max_lis;
	int i = last[j];
	lis[--j] = input[i];

	for (; j > 0; --i) {
		assert(i >= 0);
		if (length[i] != j || input[i] >= lis[j])
			continue;
		lis[--j] = input[i];
	}

	return lis;
}
