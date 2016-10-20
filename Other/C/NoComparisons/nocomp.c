#include "stdio.h"
void print(int i);
void print2(int i);

//-------------------- print --------------------
void done(int i) { }
void print_impl(int i)
{
	fputs("Hello", stdout);
	return print(--i);
}

void print(int i)
{
	static void(*funcs[2])(int) = { done, print_impl };
	return funcs[!!i](i);
}

//--------------------  test  --------------------
int main()
{
    print(0);
    putchar('\n');
    print(1);
    putchar('\n');
    print(2);
    putchar('\n');
}
