#include <stddef.h>
void (* (*FuncPtr[2])(const char))(int *, int) = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};

typedef int (* (*function)(void (* (*)(int *, int))(void (*)(int))))(void (* (*)(const char))(int *, int, void (*)(int)));

void DoNothing(void)
{
}

void UnusefulFunc()
{
	DoNothing();
}

void (*GetUnusefulPtr(const char a))(int *,int)
{
    return &UnusefulFunc;
}

void Add(int *a, int b)
{
	*a += b;
}

void Sub(int *a, int b)
{
	*a -= b;
}

void (*GetPtr(const char op))(int *,int)
{
	switch(op)
	{
		case '+':
			return &Add;
		case '-':
			return &Sub;
	}
	return NULL;
}

void (*(*GetTwoce(const char i))(const char))(int *,int)
{
	switch(i)
	{
		case 0:
			return &GetPtr;
		case 1:
			return &GetUnusefulPtr;
	}
	return NULL;
}

int main (int a, char **b)
{
	int x = 5, y = 2, z = 3;

	FuncPtr[0] = GetTwoce(0);
	FuncPtr[1] = GetTwoce(1);

	FuncPtr[0]('+')(&x, y);
	printf("%d\n", x);
	FuncPtr[0]('-')(&y, z);
	printf("%d\n", y);

	x = 5, y = 2, z = 3;
	FuncPtr[1]('+')(&x, y);
	printf("%d\n", x);
	FuncPtr[1]('-')(&y, z);
	printf("%d\n", y);
}
