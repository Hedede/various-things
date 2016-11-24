
#include <iostream>
#include <aw/io/mmap_file.h>
int main()
{
	std::cout << sizeof(wchar_t) << '\n';
	aw::io::mmap_file f( "shit" );
	std::cout << "huytevor\n";

	for (auto c : f)
		std::cout << c;
}
