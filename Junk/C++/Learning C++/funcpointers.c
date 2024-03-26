#include <stdlib.h>

void (* (*FuncPtr[8])(const char))(int *, int) = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}; 

void DoNothing (void)
{
	__asm__ ("nop");
}

void UnusefulFunc(int *a, int b)
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

int main (int, char **)
{
	void (*fptr)(int *,int) = NULL;
	int x = 1, y = 2;
	FuncPtr[0] = GetTwoce(0);
	FuncPtr[1] = GetTwoce(1);
	fptr = (*FuncPtr)('+');
	(*fptr)(&x,y);
	fptr = (*FuncPtr)('-');
	(*fptr)(&x,y);
	fptr = (*(FuncPtr+1))(NULL);
	(*fptr)(&x,y);
	return 0;
}
