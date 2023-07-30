// https://reprog.wordpress.com/2010/04/19/are-you-one-of-the-10-percent/
template<typename RandomIt, typename T>
RandomIt bsearch(RandomIt begin, RandomIt _end, T const& value)
{
	auto end = _end;

	while (begin != end) {
		auto middle = begin + (end - begin)/2;
		if (*middle < value)
			begin = ++middle;
		else if (value < *middle)
			end = middle;
		else
			return middle;
	}

	return _end;
}

template<typename RandomIt, typename T>
RandomIt lbound(RandomIt begin, RandomIt end, T const& value)
{
	while (begin != end) {
		auto middle = begin + (end - begin)/2;
		if (*middle < value)
			begin = ++middle;
		else
			end = middle;
	}

	return begin;
}

#include <vector>
#include <iostream>
int main()
{
	{
		std::vector<int> vec{ 171 };
		std::cout << *bsearch( vec.begin(), vec.end(), 171 ) << '\n';
		std::cout << *lbound(  vec.begin(), vec.end(), 171 ) << '\n';
	}
	{
		std::vector<int> vec{ 0,171,200 };
		std::cout << *bsearch( vec.begin(), vec.end(), 172 ) << '\n';
		std::cout << *lbound(  vec.begin(), vec.end(), 172 ) << '\n';
	}
	{
		std::vector<int> vec{ 0,10,12,20,23,45,57,80,90,100,140,160,171,200,220,256,700,900,1000 };
		std::cout << *bsearch( vec.begin(), vec.end(), 15 ) << '\n';
		std::cout << *lbound(  vec.begin(), vec.end(), 15 ) << '\n';
	}
}
