#include <iostream>

int move[] = {2, 3, 4, 5};

int main()
{
	int numMoves;
	for (numMoves = 0; numMoves < 6; ++numMoves) {
		if (!move[numMoves])
			break;
	}
std::cout << numMoves;
}
