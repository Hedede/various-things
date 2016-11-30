#include <iostream>
#include <fstream>
#include <unistd.h>
int main()
{
	std::string vv = "0123456789";
	auto shsc =  getenv("SSH_CLIENT");
	if (!shsc) shsc = getenv("SSH_CONNECTION");
	std::string env = shsc ? shsc : "local";
	std::string name = "fakesh_";
	for (auto c : env) {
		if (vv.find(c) != vv.npos)
			name += c;
		if (name.size() > 15) break;
	}
	std::ofstream file{"/tmp/fakesh/" + name, std::ios::app};
	if (!file)
		file.open("/tmp/fakesh/fakesh_wtf", std::ios::app);

	file << "------- " << env << '\n';

	std::string line;
	while(true) {
		std::cout << "sh$ ";
		if(!getline(std::cin, line))
			break;
		file << line << '\n';
	}
	std::cout << '\n';
}
