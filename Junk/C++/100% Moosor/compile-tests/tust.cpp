#include <iostream>

template<typename...T>
void t(int a = 0, T...t)
{
}

int main()
{
	t();
}
