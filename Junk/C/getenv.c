#include <cstdlib>
#include <string>

int main () {
	{
		int i;
		i =1;
	}

	std::string home = getenv("HOME");
	system(("echo " + home).c_str());
	return 0;
}
