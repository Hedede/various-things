#include <chrono>
#include <iostream>
#include <memory>
#include <cstring>

std::string operator*(std::string_view str, int n)
{
	std::string result(str);

	result.reserve(n);
	for (int i = 1; i <= n; i *= 2)
		result += result;

	result.resize(n);

	return result;
}

std::string gpt4(std::string_view str, int n)
{
    if (n <= 0)
        return "";

    std::string result(str);
    result.reserve(n);
    while (result.size() < n) {
        if (result.size() * 2 <= n) {
            // Duplicate the whole string if it won't exceed n
            result += result;
        } else {
            // Add only the needed part of the string to reach n characters
            result += result.substr(0, n - result.size());
        }
    }

    return result;
}

std::string mul(std::string_view str, int n)
{
	std::string result(n, 0);

	auto s = str.size();
	for (size_t i = 0; i < n; ++i) {
		result[i] = str[i%s];
	}

	return result;
}
std::string mul2(std::string_view str, int n)
{
	std::string result(n, 0);

	auto s = str.size();
	size_t i = 0, j = 0;
	while (i < n) {
		result[i++] = str[j++];
		j = j < s ? j : 0;
	}

	return result;
}

std::string mul3(std::string_view str, int n)
{
	std::string result(str);
	result.resize(n);

	size_t i = str.size(), j = 0;
	while (i < n) {
		result[i++] = result[j++];
	}

	return result;
}

std::string mul4(std::string_view str, int n)
{
	std::string result(n, 0);

	auto s = str.size();
	size_t i = 0;
	while (i+s < n) {
		memcpy( result.data()+i, str.data(), s );
		i+=s;
	}

	size_t j = 0;
	while (i < n) {
		result[i++] = str[j++];
	}

	return result;
}

std::string pul4(std::string_view str, int n)
{
	std::string result(str);
	result.resize(n);
	auto* p = result.data();
	auto s = str.size();
	size_t i = s;
	while (i*2 < n) {
		memcpy( p+i, p, i );
		i*=2;
	}

	if (i < n)
		memcpy( p+i, p,  n - i );

	 return result;
}
std::string pul5(std::string_view str, int n)
{
	auto p = new char[n];
	memcpy( p, str.data(), std::min(n,(int)str.size()) );
	//auto* p = result.data();
	auto s = str.size();
	size_t i = s;
	while (i*2 < n) {
		memcpy( p+i, p, i );
		i*=2;
	}

	if (i < n)
		memcpy( p+i, p,  n - i );

	 std::string r( p, n);
	 delete[] p;
	 return r;
}
std::string repeat1(std::string_view str, size_t n)
{
	if (n == 0)
		return {};

	std::string result(str);
	if (n == 1)
		return result;

	size_t size = result.size();
	const auto total_size = size*n;

	result.resize(total_size);

	auto* p = result.data();

	while (size*2 < n) {
		memcpy( p+size, p, size );
		size*=2;
	}

	 return result;
}
std::string repeat(std::string_view str, size_t n)
{
	if (n == 0)
		return {};

	std::string result(str);
	if (n == 1)
		return result;

	size_t size = result.size();
	const auto total_size = size*n;

	result.resize(total_size);

	auto* p = result.data();

	while (size*2 < n) {
		memcpy( p+size, p, size );
		size*=2;
	}

	 return result;
}

void donotopt(std::string&);

using namespace std::chrono;
int rep1=100;
int rep2=5000000;
int rep3=1000000000;
template<typename T>
void test1(std::string_view name, T t)
{
	std::string s = "====-====";
	auto t1 = steady_clock::now();

	for (int i = 0; i < rep2; ++i) {
		auto m = t(s , rep1);
		donotopt(m);
	}

	auto t2 = steady_clock::now();

	std::cout<< "LOT:" << name << ' ' << duration<double,std::milli>(t2-t1).count() << "ms\n";
}

template<typename T>
void test2(std::string_view name, T t)
{
	std::string s = "====-====";
	auto t1 = steady_clock::now();

		auto m = t(s, rep3);
		donotopt(m);

	auto t2 = steady_clock::now();

	std::cout <<"BIG:"<< name << ' ' << duration<double,std::milli>(t2-t1).count() << "ms\n";
}

int main()
{
	test1( "gpt4", gpt4 );
	test2( "gpt4", gpt4 );

	test1( "pul4", pul4 );
	test2( "pul4", pul4 );

	test1( "pul5", pul5 );
	test2( "pul5", pul5 );
}
