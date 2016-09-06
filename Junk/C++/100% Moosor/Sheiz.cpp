#include <iostream>
struct oth {
int x;
};
struct zstr {
int x;
std::string z;
};

int prlont(zstr& z)
{
	std::cout << z.z;
}

struct govn : zstr, oth {
int y;
void archive(){
	prlont(*this);
}
};


int main()
{
	govn g;
	g.z = "\"asd\"\n";

	g.archive();
}
