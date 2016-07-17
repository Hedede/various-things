#include <iostream>
#include <time.h>
#include <windows.h>

using namespace std;

int main() {
input:
	srand(time(0));
	int z = rand();
	Sleep(z % 477);
	srand(time(0));
	int y = rand();
	Sleep((z + y) % 533);
	srand(time(0));
	int x = rand();
	int i = (z - y + x) % 202;
	system("cls");
	cout  << "rand() = "  << z << endl
	      << "% 1 = "     << z % 1 << endl
	      << "% 2 = "     << z % 2 << endl
	      << "% 3 = "     << z % 3 << endl
	      << "% 4 = "     << z % 4 << endl
	      << "% 5 = "     << z % 5 << endl
	      << "% 6 = "     << z % 6 << endl
	      << "% 10 = "    << z % 10 << endl
	      << "% 25 = "    << z % 25 << endl
	      << "% 73 = "    << z % 73 << endl
	      << "% 202 = "   << z % 202 << endl
	      << "% 2048 = "  << z % 2048 << endl
	      << "% 99999 = " << z % 99999 << endl
	      << "x y z = "   << i << endl;
	//system("pause");
	goto input;
}
