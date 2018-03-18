#include <iostream>
#include "queue.h"

#include <vector>
#include <deque>
#include <string>

struct String {
	String(int i)
		: a(std::to_string(i))
	{
		//a = i;
		//b = 2*i;
		//c = 3*i;
		//d = 4*i;
	}

	std::string a;
	//long a,b,c,d;
};

bool operator==(String a, String b)
{
	return a.a == b.a;
}

std::ostream& operator<<(std::ostream& o, String const& s)
{
	return o << s.a;
}

int main()
{
	using namespace aw;
	using namespace std;
	queue<String> k;
	deque<String> v;

	auto pushb = [&] (int i) {
		k.push_back(i);
		v.push_back(i);
	};

	auto pushf = [&] (int i) {
		k.push_front(i);
		v.push_front(i);
	};

	auto popb = [&] {
		k.pop_back();
		v.pop_back();
	};

	auto popf = [&] () {
		k.pop_front();
		v.pop_front();
	};

	for (unsigned s = 0; s < 17; ++s) {
		pushb(s);
	}

	unsigned m = k.size();
	for (unsigned s = 0; s != m; ++s) {
		assert(k.front() == v.front());
		popf();
	}

	assert(k.empty());
	assert(v.empty());

	pushf(0);
	pushf(0);
	pushf(0);
	pushf(0);
	pushf(11);
	pushb(10);
	pushf(-11);
	pushf(10);
	pushb(15);
	popf();
	pushb(2);
	popb();
	pushb(4);
	pushb(4);
	pushf(7);
	popb();
	popb();
	pushf(22);
	pushf(15);
	pushf(17);
	pushb(66);
	pushb(10);
	pushb(12);
	pushb(0);
	pushb(-55);
	pushb(12);
	pushb(-12);
	popf();
	pushb(12);
	pushb(-14);
	pushb(-12);
	popf();
	pushb(-12);
	pushb(12);
	popf();
	pushb(-12);
	pushb(-12);
	pushf(16);
	pushf(16);
	pushf(16);
	pushb(-12);
	pushb(-12);
	pushf(10);
	pushb(-12);
	pushf(10);
	pushb(-12);
	pushf(100);
	popf();
	pushb(100);
	pushf(100);

	auto beg_k = std::begin(k);
	auto end_k = std::end(k);

	auto beg_v = std::begin(v);
	auto end_v = std::end(v);

	unsigned count = 0;

	for (; beg_k != end_k; ++beg_k, ++beg_v, ++count) {
		assert(*beg_k == *beg_v);
		std::cout << *beg_k << " " << *beg_v << "\n";
	}

	assert(count == v.size());

	assert(beg_k == end_k);
	assert(beg_v == end_v);

	std::cout << k.capacity() << "\\" << k.size() << "\n";
	std::cout << count << "\n";

	assert(count == k.size());

	k.shrink_to_fit();

	std::cout << k.capacity() << "\\" << k.size() << "\n";


	std::cout << std::begin(k).map() << ":nigeb\n";
	beg_k = std::begin(k);
	end_k = std::end(k);
	count = 0;
	for (; beg_k < end_k; ++beg_k, ++count) {
		std::cout << beg_k.map() << ":" << count << "\n";
	}
	std::cout << std::end(k).map() << ":dne\n";
}
