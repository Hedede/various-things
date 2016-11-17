#include <aw/utility/string/split.h>
#include <aw/utility/random/mersenne_twister.h>
#include <aw/utility/test.h>

TestFile( "Random tests" );

namespace aw {
constexpr char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
constexpr char separators[] = " ,./\\|<>?!@#$%^&*()-=_+[]{}";

Test(split_random) {
	std::vector< std::string > strs;
	std::vector< std::vector<std::string> > results;
	std::vector< std::vector<std::string> > expect;

	std::mt19937 engine = create_mersenne_twister_engine<std::mt19937>();

	Setup {
		for (size_t n = 0; n < 50; ++n) {
			std::string s;
			expect.emplace_back();
			char sep = separators[engine() % (sizeof(separators)-1)];
			s.append(1,sep);
			for (size_t n = 0, e = (1 + engine() & 0xF); n < e; ++n) {
				s.reserve((e+1)*0xF);
				std::string t;
				for (size_t n = 0, e = (1 + engine() & 0xF); n < e; ++n)
					t.append(1,alphabet[engine() % (sizeof(alphabet)-1)]);
				if (!t.empty()) {
					expect.back().push_back(t);
					s.append(t);
					s.append(1,sep);
				}
			}
			strs.push_back(std::move(s));
		}
	}

	Preconditions {}

	Checks {
		for (auto& s : strs)
			results.push_back(string::split(s, {1,s[0]}));
		while (!results.empty()) {
			std::cout << "++++++++++++++++++";
			for (auto& v : results.back())
				std::cout << v << ' ';
			std::cout << '\n';
			std::cout << "------------------";
			for (auto& v : expect.back())
				std::cout << v << ' ';
			std::cout << '\n';
			TestAssert(results.back() == expect.back());
			results.pop_back();
			expect.pop_back();
		}
		
	}

}
} // namespace aw
