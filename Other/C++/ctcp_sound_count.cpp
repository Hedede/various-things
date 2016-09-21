//#include <map>
//#include <set>
#include <cctype>
#include <iostream>
//#include <iomanip>
#include <string>
#include <algorithm>
#include <experimental/filesystem>
#include <boost/container/flat_map.hpp>
#include <boost/container/flat_set.hpp>
namespace fs = std::experimental::filesystem;
namespace ebat = boost::container;

int main()
{
	ebat::flat_map<std::string, unsigned> sounds;
	ebat::flat_map<std::string, unsigned> sound_lines;
	ebat::flat_map<std::string, unsigned> sound_first;
	ebat::flat_set<std::string> actual_sounds;
	for (auto p : fs::directory_iterator("mIRCounds")) {
		if (p.path().extension().string() != ".wav") continue;
		std::string name = p.path().stem().string();
		actual_sounds.insert(name);
		sounds.emplace(name, 0);
	}

	unsigned line = 0;
	std::string line_str;
	std::cerr << "line blyad: ";
	std::cerr << '0';
	std::string fuck;
	while(std::getline(std::cin, fuck)) {
		std::string erase = std::string(line_str.size()+1, '\b');
		line_str = std::to_string(++line);
		std::cerr << erase << " " << line_str;
		if (fuck.size() < 12) continue;
		if (!std::isalnum(fuck[11])) continue;
		auto pos1 = fuck.find(".wav");
		if (pos1 == std::string::npos) continue;
		auto pos2 = fuck.rfind(' ', pos1);
		auto name = fuck.substr(pos2+1, pos1-pos2-1);
		++sounds[name];
		sound_lines[name] = line;
		if (sound_first.find(name) == end(sound_first))
			sound_first[name] = line;
	};

	auto ctr = [] (char c) {
		return !((c & 0xC0) == 0x80);
	};

	auto count_leading = [&ctr] (std::string const& s) {
		return std::count_if(begin(s), end(s), ctr);
	};

	auto longest_2nd = std::max_element(begin(actual_sounds), end(actual_sounds),
		[&count_leading] (auto& a, auto& b) {
			auto ac = count_leading(a);
			auto bc = count_leading(b);
			return ac < bc;
		}
	);
	auto longest_3rd = std::max_element(begin(sound_lines), end(sound_lines),
		[] (auto& a, auto& b) {
			return std::to_string(a.second).size() <
			       std::to_string(b.second).size();
		}
	);

	size_t _2nd = count_leading(*longest_2nd);
	size_t _3rd = std::to_string(longest_3rd->second).size();

	ebat::flat_multimap<unsigned, std::string> huy;
	for (auto& kv : sounds) {
		if (actual_sounds.find(kv.first) == end(actual_sounds)) continue;
		huy.insert(std::make_pair(kv.second, kv.first));
	}

	for (auto& kv : huy) {
		std::string _1 = std::to_string(kv.first);
		_1.resize(5, ' ');
		std::string _2 = kv.second;
		size_t pad = _2nd - count_leading(_2) + 1;
		_2.append(pad, ' ');
		std::string _3 = std::to_string(sound_first[kv.second]);
		_3.insert(0, _3rd - _3.size() + 1, ' ');
		std::string _4 = std::to_string(sound_lines[kv.second]);
		//_4.insert(0, _3rd - _4.size(), ' ');
		std::cout << _1 << ' ' << _2 << ' ' << _3 << '-' << _4 << '\n';
	}
}
