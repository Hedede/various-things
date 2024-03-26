#include <iostream>
#include <cstdlib>

void No3 () 
{
    using std::cout;
    using std::endl;
	int x = 55;
	int y = 75;
	while(x != y) {
		if(x > y) {
			x = x - y;
		} else {
			y = y - x;
		}
		cout << "x = " << x << ", " << "y = " << y << endl;
	}
}

void No4 () 
{
    using std::cout;
    using std::endl;
	int x = 10;
	int y = 15;
	while(y < 16) {
		if(x <= y) {
			x += 5;
			y -= 5;
		} else {
			x -= 3;
			y += 5;
		}
		cout << "x = " << x << ", " << "y = " << y << endl;
	}
}

void No5 () 
{
    using std::cout;
    using std::endl;
	int a = 16;
	int b = 2;
	while (b != 32) {
		b *= 2;
		a += 2;
		cout << "a = " << a << ", " << "b = " << b << endl;
	}
}

void No6 () 
{
    using std::cout;
    using std::endl;
	int n = 10;
	int m = 12;
	while (m > 6) {
		m -= 2;
		n *= 2;
		cout << "n = " << n << ", " << "m = " << m << endl;
	}
}

void No7 () 
{
    using std::cout;
    using std::endl;
	int x = 15;
	int y = 35;
	while(x < 30) {
		if(x >= y) {
			x -= 5;
			y += 5;
		} else {
			x += 10;
			y -= 10;
		}
		cout << "x = " << x << ", " << "y = " << y << endl;
	}
}

char* n = "151515";

class someclass
{
	public:
		virtual void somevirtualfunc() {};

};

class someclass2 : someclass
{
	public:
		virtual void somevirtualfunc() {};

};

typedef void (someclass2::*fptr)();

void Test()
{
	someclass2 ololo;
	ololo.somevirtualfunc();
	fptr ptr;
	ptr = &someclass2::somevirtualfunc;
}

int main(int, char**)
{
    using std::cout;
    using std::endl;
	char* ololo = new char[32];
	const char lolol[] = "arara ololo grololo";
	unsigned int i = 0;
	for ( i; *(lolol+i); i++ ) *(ololo+i) = *(lolol+i);

	ololo[11] = 'A';

	int s = 0, k = 0;
	while(k<30)
	{
		k += 3;
		s += k;
	}

	cout << s;

	/*cout << "#3." << endl;
	No3 ();

	cout << "#4." << endl;
	No4 ();

	cout << "#5." << endl;
	No5 ();

	cout << "#6." << endl;
	No6 ();

	cout << "#7." << endl;
	No7 ();*/
	
	system("pause");
	return 0;
}