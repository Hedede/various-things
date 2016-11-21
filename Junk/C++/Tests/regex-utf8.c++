#include <regex>
#include <iostream>
// lies overflow
bool test_unicode()
{
	std::locale old;
	std::locale::global(std::locale("en_US.UTF-8"));

	std::regex pattern("[[:alpha:]]+", std::regex_constants::extended);
	bool result = std::regex_match(std::string("abcdéfg"), pattern);

	std::locale::global(old);

	return result;
}
int main()
{
	{
		std::locale::global(std::locale("ru_RU.UTF-8"));
		std::regex reg{u8"с[оа]ундрепорт", std::regex_constants::egrep};
		std::cout<<std::regex_search(u8"дерп дай соундрепорт", reg)<<'\n';
	}
	{
		std::regex reg{u8"соундрепорт",
			std::regex_constants::egrep|std::regex_constants::icase};
		std::cout<<std::regex_search(u8"дерп дай СОУНДРЕПОРТ", reg)<<'\n';
	}
	{
		std::wregex reg{L"соундрепорт",
			std::regex_constants::egrep|std::regex_constants::icase};
		std::cout<<std::regex_search(L"дерп дай СОУНДРЕПОРТ", reg)<<'\n';
	}
	{
		std::regex reg{u8"с[а-о]ундрепорт", std::regex_constants::egrep};
		std::cout<<std::regex_search(u8"дерп дай соундрепорт", reg)<<'\n';
	}
	{
		std::regex reg{u8"с(о|а)ундрепорт", std::regex_constants::egrep};
		std::cout<<std::regex_search(u8"дерп дай соундрепорт", reg)<<'\n';
	}
	{
		std::regex reg{u8"p[oa][ei]ben", std::regex_constants::egrep};
		std::cout<<std::regex_search(u8"kupi paiben", reg)<<'\n';
	}
	{
		std::wregex reg{L"с[оа]ундрепорт", std::regex_constants::egrep};
		std::cout<<std::regex_search(L"дерп дай соундрепорт", reg)<<'\n';
	}
	std::cout << test_unicode();
}
