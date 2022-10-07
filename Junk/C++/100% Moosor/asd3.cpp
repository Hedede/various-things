#include <iostream>
#include <thread>


std::string g;
bool tru;

bool kostul;
std::istream& getlinep(std::istream& stream, std::string& str)
{
	char ch;
	str.clear();
	kostul = true;
	std::cout << kostul;
	ch = stream.get();

	std::cout << ch;
	kostul = false;
	std::cout << kostul;
	while (ch != '\n') {
		str.push_back(ch);
		stream.get(ch);
	}
	return stream;
}

void getliner()
{
	while(true) {
		std::cout.flush();
		if (!kostul) continue;
	//	std::cout << "nagluj";// << std::endl;
	}
}


int main()
{
	std::thread th(getliner);
	while(true) {
		getlinep(std::cin, g);
	}
	th.join();
}
