#include <iostream>
#include <string>

void excpec()
{
#if __cpp_exceptions
	int i;
	try {
		i = std::stoi("1000000000");
	} catch(...) {
		std::cout << "caught bullshit\n";
	}
#endif
}

void nexcpec() noexcept
{
	std::cout << "noexcept";
}


int main()
{
	nexcpec();

}
