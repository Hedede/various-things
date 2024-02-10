/*
 * Copyright (C) 2014-2015  absurdworlds
 * Copyright (C) 2015       Hedede <hededrk@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#include <vector>
#include <string>


static char const CompositionChar = '%';

std::string compose(
        std::string const& fmt,
        std::vector<std::string> const& args)
{
	char const delim = CompositionChar;

#if 0
	static std::string result;
	result.clear();
#else
	std::string result;
	//result.reserve(fmt.size());
#endif

	size_t pos = 0;

	while (pos != std::string::npos) {
		size_t nextpos = fmt.find(delim, pos);

		result += fmt.substr(pos, nextpos - pos);
		if (nextpos == std::string::npos)
			break;

		char idx = fmt[++nextpos];
		if (!isdigit(idx)) {
			if (idx == delim) {
				result += delim;
				++nextpos;
			} else {
				result += delim;
			}
		} else {
			pos = nextpos;

			while (isdigit(fmt[nextpos]))
				++nextpos;

			size_t arg_no = stoull(fmt.substr(pos, nextpos - pos));

			if (arg_no < args.size())
				result += args[arg_no];
		}
		pos = nextpos;
	}

	return result;
}


[[gnu::used, gnu::noinline]] void use(std::string&& str) {}

int main()
{
	using namespace std::string_literals;
	using string_vec = std::vector<std::string>;
	auto af = "%0%1"s;
	auto bf = "%0 %1 %2"s;
	auto cf = "just some string: %0"s;
	auto df = "just some string: %0%0"s;
	auto ef = "%1%2%3%4"s;
	string_vec av = {"a "s, "striiiiiing"s};
	string_vec bv = {"a"s, "veeeeeeerrrrrrrry, verrrrrrrry, veeeeerrrrrryyyy, loooooooong", "striiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiing"s};
	string_vec cv = {"hmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm"s};
	string_vec dv = {"hmm"s, "long long long long long longloongloongloongloongloongloongloongloongloongloongloongloongloongloongloongloongloongloongloongloongloongloongloongloongloongloongloongloongloongloong long"s};
	string_vec ev = {"hmm"s, "long long long long long longloongloongloongloongloongloongloongloongloongloongloongloongloongloongloongloongloongloongloongloongloongloongloongloongloongloongloongloongloongloong long"s, std::string(10000u, 'a'), "123213215468579465454121654987748974984561321564987987489645416513654687968749749681496485361432168779496851513464876991846587498122492+682485624154684156874568421541454958495847685745468471627154616852416542164521463528165475448714652464281446524584256824684682745687249684568742546847429187840187540874910456145881490546841009851212503254278104561980714062741356242106032250143674325016108168296197610271276"s};

	for (auto i = 0u; i < 500000u; ++i) {
		use(compose(af, av));
		use(compose(bf, bv));
		use(compose(cf, cv));
		use(compose(df, dv));
		use(compose(ef, ev));
	}
}
