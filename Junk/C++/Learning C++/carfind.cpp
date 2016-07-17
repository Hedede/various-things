#include <iostream>
#include <windows.h>
#include <string.h>
#include <time.h>

using namespace std;

char vehnames[][20] = {"Ucoz", "Dranduled", "Dran Ucozom duled"};

void findcar(char const* findname) {
	int len = strlen(findname); 
	int cars = sizeof(vehnames);
	bool win = false;
	for(int i = 0; i <= cars; i++) {
		if(!strncmp(findname, vehnames[i], len)) {
			cout << "car id:" << i + 400 << ", car name: \"" << vehnames[i] << "\"\n";
			win = true;
		}
	}
	win ? cout << "> dovolen, kukufcheg?\n" : cout << "> nenashol mya tojego ucoza\n";
	return win
}

int main() {
input:
	std::string name;
	std::cin >> name;

	findcar(name.c_str());
	system("pause");
	goto input;
}
