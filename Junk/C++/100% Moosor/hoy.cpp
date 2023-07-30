#include <dirent.h>

int main()
{
	return opendir("Dropbox/") != 0;
}
