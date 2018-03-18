#include <windows.h>
#include <iostream>

using namespace std; 

int main(void) 
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdout == INVALID_HANDLE_VALUE) 
	{
		cout << "Error while getting input handle" << endl;
		return EXIT_FAILURE;
	}
	//sets the color to intense red on blue background
	SetConsoleTextAttribute(hStdout, FOREGROUND_RED | BACKGROUND_BLUE | FOREGROUND_INTENSITY);

	cout << "This is intense red text on blue background" << endl;
	//reverting back to the normal color
	SetConsoleTextAttribute(hStdout, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	cin >> hStdout;

	return EXIT_SUCCESS;
}
