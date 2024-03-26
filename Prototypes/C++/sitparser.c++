// converts .SIT into my own format
#include <fstream>
#include <iostream>
#include <aw/utility/string/split.h>

void strip_junk( std::string& str)
{
	auto pos = str.find_first_not_of(" \t");
	str.erase(0, pos);
	pos = str.find('/');
	if (pos != str.npos)
		str.resize(pos);
	pos = str.find_last_not_of(" \t\r");
	str.resize(pos+1);
}

void parse_pickup( std::istream& is )
{
	std::string str[11];
	for (auto& s : str) {
		getline( is, s );
		strip_junk(s);
	}

	std::cout << "[pickup\n";
	std::cout << "\tname = "       << str[0] << '\n';
	std::cout << "\tstaticName = " << str[1] << '\n';
	std::cout << "\tsize = {"      << str[2] << "}\n";
	std::cout << "\tpos = {"       << str[3] << "}\n";
	std::cout << "\trot = {"       << str[4] << "}\n";
	//std::cout << "\tmass = "       << str[5] << '\n';
	if (str[6] != "\"\"")
	std::cout << "\tsfx = "        << str[6] << '\n';
	if (str[7] != "\"\"")
	std::cout << "\tcontwav = "    << str[7] << '\n';
	if (str[8] != "0")
	std::cout << "\tparent = "     << str[8] << '\n';
	if (str[9] != "0")
	std::cout << "\titemType = "   << str[9] << '\n';
	//std::cout << "\tunlocked = "   << str[10] << '\n';
	std::cout << "]\n";
}

void parse_collidable( std::istream& is )
{
	std::string str[12];
	for (auto& s : str) {
		getline( is, s );
		strip_junk(s);
	}

	std::cout << "[collidable\n";
	if (str[0] == "\"\"")
	std::cout << "\tname = "       << str[0] << '\n';
	std::cout << "\tstaticName = " << str[1] << '\n';
	std::cout << "\tsize = {"      << str[2] << "}\n";
	std::cout << "\tpos = {"       << str[3] << "}\n";
	std::cout << "\trot = {"       << str[4] << "}\n";
	//std::cout << "\tmass = "       << str[5] << '\n';
	// all without sound
	//std::cout << "\tsfx = "        << str[6] << '\n';
	//std::cout << "\tcontwav = "    << str[7] << '\n';
	//std::cout << "\tparent = "     << str[8] << '\n';
	// all 0.0333333
	//std::cout << "\ttimePerFrame   = "   << str[9] << '\n';
	// all 0
	//std::cout << "\tcastShadowOnMe = " << str[10] << '\n';
	//std::cout << "\tcollisionType  = "   << str[11] << '\n';
	std::cout << "]\n";
}

void parse_breakble( std::istream& is )
{
	std::string str[21];
	for (auto& s : str) {
		getline( is, s );
		strip_junk(s);
	}

	std::cout << "[breakable\n";
	// std::cout << "\tname = "        << str[20] << '\n';
	std::cout << "\tstaticName = "  << str[0]  << '\n';
	std::cout << "\tsize = {"       << str[1]  << "}\n";
	std::cout << "\tpos  = {"        << str[2]  << "}\n";
	std::cout << "\trot  = {"        << str[3]  << "}\n";
	if (str[4] != "0")
	std::cout << "\tmass = "        << str[4]  << '\n';
	if (str[5] != "\"\"")
	std::cout << "\tsfx = "         << str[5]  << '\n';
	if (str[6] != "\"\"")
	std::cout << "\tcontwav = "     << str[6]  << '\n';
	// std::cout << "\tparent = "      << str[7]  << '\n';
	std::cout << "\thitpoints = "   << str[8]  << '\n';
	// std::cout << "\tdamageState = " << str[9]  << '\n';
	if (str[10] != "\"\"") {
	std::cout << "\tconstEffect = "      << str[10]  << '\n';
	std::cout << "\tconstEffectOff = {"  << str[13]  << "}\n";
	std::cout << "\tconstEffectFwd = {"  << str[14]  << "}\n";
	}
	if (str[11] != "\"\"") {
	std::cout << "\tdeathEffect = "      << str[11]  << '\n';
	std::cout << "\tdeathEffectOff = {"  << str[15]  << "}\n";
	std::cout << "\tdeathEffectFwd = {"  << str[16]  << "}\n";
	}
	//std::cout << "\tflyingEffect = "    << str[12] << '\n';
	//std::cout << "\texplosionRadius = " << str[17] << '\n';
	//std::cout << "\texplosionDamage = " << str[18] << '\n';
	//std::cout << "\texplosionForce = "  << str[19] << '\n';
	std::cout << "]\n";
}

#include <map>
int main()
{
	std::ifstream amuse{"AMUSE.SIT"};
	std::map<std::string, void(*)(std::istream&)> parsogon = {
		{ "CPickUpItem", parse_pickup },
		{ "CCollide",    parse_collidable  },
		{ "CBreakable",  parse_breakble }
	};
	auto exec = [&] (aw::string_view name) {
		auto pp = parsogon.find( (std::string)name );
		if (pp != end(parsogon))
			return (pp->second)( amuse );
	};
	std::string line;
	while (getline(amuse, line)) {
		if (line.empty())
			continue;
		if (line[0] == '{') {
			auto vec = aw::string::split_by(line, " \r");
			auto type = vec[2];
			exec(type);
			//std::cerr << type << '\n';
		}
	}
}
