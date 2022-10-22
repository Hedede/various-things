#include <vector>
#include <map>
#include <iostream>
#include <string_view>
#include <charconv>
#include <aw/algorithm/in.h>


std::map<int,int> prec = {
	{'-', 0},
	{'-', 0},
	{'*', 1},
	{'/', 1},
};

std::string eval_expr(std::string_view s, std::map<std::string, int, std::less<>>& vars)
{
	std::vector<int> op_stack;
	std::vector<int> data_stack;

	auto eval = [&] {
		auto op = op_stack.back(); op_stack.pop_back();
		if (data_stack.size() < 2)
			return;
		auto r = data_stack.back(); data_stack.pop_back();
		auto l = data_stack.back(); data_stack.pop_back();
		if (op == '-')
		{
			data_stack.push_back(l - r);
		}
		else if (op == '+')
		{
			data_stack.push_back(l + r);
		}
		else if (op == '*')
		{
			data_stack.push_back(l * r);
		}
		else if (op == '/')
		{
			data_stack.push_back(l / r);
		}
	};

	for (auto i = 0; i < s.size(); ++i)
	{
		auto c = s[i];
		if (c == ' ')
			continue;

		if (c == '(')
		{
			op_stack.push_back('(');
			continue;
		}

		if (c == ')')
		{
			while (!op_stack.empty() && op_stack.back() != '(')
			{
				eval();
			}
			op_stack.pop_back();
			continue;
		}

		if (aw::in(c, '-', '+', '*', '/'))
		{
			while (!op_stack.empty() && prec[c] <= prec[op_stack.back()])
			{
				eval();
			}
			op_stack.push_back(c);
			continue;
		}

		if (c >= '0' && c <= '9')
		{
			auto start = i;
			do
			{
				c = s[++i];
			}
			while (c >= '0' && c <= '9');

			int v;
			std::from_chars(s.data()+start, s.data()+i, v);
			data_stack.push_back(v);

			--i;
			continue;
		}

		if (c == '$')
		{
			c = s[++i];
			if (c == '{')
			{
				auto start = ++i;
				for (; i < s.size();)
				{
					c = s[i];
					if (c == '}')
						break;
					++i;
				}

				auto x = s.substr(start, i - start);

				auto it = vars.find(x);
				if (it != vars.end())
					data_stack.push_back(it->second);
			}
		}

	}

	while (!op_stack.empty())
	{
		eval();
	}

	std::string result;
	for (auto v : data_stack)
		result += std::to_string(v) + ' ';
	if (result.size())
		result.pop_back();

	return result;
}

int main(int, char*  *argv)
{
	std::string_view s = argv[1];

	std::map<std::string, int, std::less<>> vars = {
		{ "a", 10 },
		{ "b", 8 },
	};

	std::cout << eval_expr(s, vars);
}
