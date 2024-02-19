#include <iostream>
#include <cstdlib>
using namespace std;

int main () {

	cout << std::getenv("REQUEST_METHOD") << '\n';

	cout << "Content-type:text/html\r\n\r\n";
	cout << "<html>\n";
	cout << "<head>\n";
	cout << "<title>Zis page is written in C++</title>\n";
	cout << "</head>\n";
	cout << "<body>\n";
	cout << "<h2>Zis piece of crap is written in C++. Eedeete nahooi php-koderyyy!</h2>\n";
	cout << "</body>\n";
	cout << "</html>\n";

	return 0;
}
