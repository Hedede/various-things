#if defined(__GNUG__)
#define NULL __null
#elif defined(__cplusplus)
#define NULL 0
#else
#define NULL ((void *)0)
#endif

void (* (*FuncPtr[8])(const char))(int *, int) = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};

typedef int (* (*function)(void (* (*)(int *, int))(void (*)(int))))(void (* (*)(const char))(int *, int, void (*)(int)));

function BrainFuck;

void DoNothing (void)
{
	#if defined(__GNUC__)
		__asm__ ("nop");
	#elif defined(_MSC_VER)
		__asm
		{
			nop
		}
	#else
	#endif
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
/*
int main (int, char **)
{
	void (*fptr)(int *,int) = NULL;
	int x = 5, y = 2, z = 3;
	FuncPtr[0] = GetTwoce(0);
	FuncPtr[1] = GetTwoce(1);
	fptr = (*FuncPtr)('+');
	(*fptr)(&x,y);
	fptr = (*FuncPtr)('-');
	(*fptr)(&x,z);
	fptr = (*(FuncPtr+1))(NULL);
	(*fptr)(&x,y);
	(*fptr)(NULL,NULL);
	return (unsigned long long)&x;
}*/

int main (int, char **)
{
	void *ptr1;
	void *ptr2;

	ptr1 = &ptr2;
	ptr2 = &ptr1;
	
	void *ptr3;
	ptr3 = ptr2;
	/* *ptr3 = &ptr3; не дает, гад */

	return 0;
}