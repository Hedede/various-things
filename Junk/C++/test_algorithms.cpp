#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string.h>
#include <stdio.h>

template <typename T>
void SwapPtr (T** x, T** y)
{
	T* z = *x;
	*x = *y;
	*y = z;
}

template <>
void SwapPtr (void** x, void** y)
{
	void* z = *x;
	*x = *y;
	*y = z;
}

template <typename T>
void Swap (T* x, T* y)
{
	T z = *x;
	*x = *y;
	*y = z;
}

#define N 26	

void countchars ()
{
	char *a = (char *)malloc(sizeof(char)*128);
	int b[N];
	
	scanf("%s",&a);


}

int main(int, char**)
{
    using std::cout;
    using std::endl;
	
	countchars ();
	system("pause");
	

	int *a = new int[4];
	int *b = new int[5];
    a[0] = 1;
    a[1] = 2;
    a[2] = 3;
    a[3] = 0;

    b[0] = 9;
    b[1] = 8;
    b[2] = 7;
    b[3] = 6;
    b[4] = 0;

	SwapPtr<int>(&a, &b);
	Swap<int>(a, b);

    do {
        cout << *a << endl;
    } while(*(a++));

	char *a1 = new char[24];
	char *b1 = new char[24];

	strcpy(a1,"This is yukhanka!");
	strcpy(b1,"This is not yukhanka!");

	SwapPtr<char>(&a1, &b1);

    cout << a1 << endl;
    cout << b1 << endl;

	strcpy(b1,"It may be a yukhanka, but it also may be a lypiivka!");
	strcpy(a1,"This is not yukhanka!");

    *(int*)((void*)(a1+48)) = 0x64638485;
    *(int*)((void*)(a1+53)) = 1;
    *(float*)((void*)(a1+12)) = 1.0177771;

    cout << b1 << endl;
    cout << a1 << endl;
	return (int)((int*)(a1+53));
}
