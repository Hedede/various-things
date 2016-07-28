#include <iostream>
#include <utility>

int matrix[4][4] = {
 {1,2,3,4},
 {9,4,7,5},
 {5,7,4,4},
 {5,2,5,4},
};

int matrix2[4][4] = {
 {1,21,11,5},
 {13,7,7,5},
 {9,7,6,5},
 {6,2,5,5},
};
template<size_t...I,size_t...J>
void print(std::index_sequence<I...>,std::index_sequence<J...>)
{
	int dummy[] = {matrix[I]...)[J]...
	};
}

int main()
{
	print(std::make_index_sequence<4>{}, std::make_index_sequence<4>{});
}
