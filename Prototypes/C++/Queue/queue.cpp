#include <algorithm>
#include <iostream>
#include <sstream>

using u = unsigned;

struct queue {
	u begin = 0, end = 0, head = 0, tail = 0;
	bool kokd[64];

	u map(u p)
	{
		if (p >= head)
			return begin + (p - head);
		return p + (end - head);
	}

	u real_capacity()
	{
		return end - begin;
	}

	u capacity()
	{
		u cap = real_capacity();
		return cap ? cap - 1 : 0;
	}

	void grow()
	{
		u cap = real_capacity();
		u sz = cap + std::max(cap, 16u);
		u rl = size();

		begin = 0;
		end = sz;

		head = begin;
		tail = rl;
	}

	u prev(u p)
	{
		if (p == begin)
			return end - 1;
		return p - 1;
	}

	u next(u p)
	{
		if (p + 1 == end)
			return begin;
		return p + 1;
	}

	u size()
	{
		return map(tail) - map(head);
	}

	void push_back()
	{
		if (size() == capacity())
			grow();
		tail = next(tail);
	}

	void push_front()
	{
		if (size() == capacity())
			grow();
		head = prev(head);
	}

	void pop_front()
	{
		head = next(head);
	}

	void pop_back()
	{
		tail = prev(tail);
	}

	void print(bool reel = false)
	{
	// Yes, print() is a mess. That's intentional

#ifdef _WINDOWS
		static std::string white;
		static std::string def;
#else
		static std::string white = "\033[1m";
		static std::string def = "\033[0m";
#endif

		std::ostringstream os;
		os << size() << "/" << capacity();

		u x = os.str().size();
		u z = 8; // xxx/xxx 
		if (x < z)
			os << std::string(z-x, ' ');

		std::cout << os.str() << "[";

		auto prt = [&] (u p) {
			return (reel ? map(p) : p);
		};

		if (head > tail)
			std::cout << white;
		for (u p = begin; p != end; ++p) {
			if (p == head && p == tail) {
				if (p!=begin)
					std::cout<< " ";
				std::cout << "|";
				std::cout << prt(p);
				if (p+1 != end)
				std::cout << " ";
			} else if (p == head) {
				std::cout << white;
				std::cout << "(";
				std::cout << prt(p);
				if (p+1 != tail && p+1 != end)
				std::cout << " ";
			} else if (p == tail) {
				std::cout << ") ";
				std::cout << def;
				std::cout << prt(p);
				if (p+1 != end)
					std::cout << " ";
			} else {
				std::cout << prt(p);
				if (p+1 != tail && p+1 != end)
				std::cout << " ";
			}
		}
		if (end == tail && end != begin)
			std::cout << ")";

		std::cout << def;
		std::cout << "] " << end << "\n";
	}

	void pmap()
	{
		for (u p = begin; p != end; ++p) {
			std::cout << map(p) << " ";
		}
		std:: cout << "\n";
	}
};

int main(int,char**ar)
{

	queue q;
	q.print();

	bool reel = 0;
	char op;

	auto front = [&] {
		if (op == '+')
			q.push_front();
		if (op == '-')
			q.pop_front();
		q.print(reel);
	};

	auto back = [&] {
		if (op == '+')
			q.push_back();
		if (op == '-')
			q.pop_back();
		q.print(reel);
	};

	auto push = [&] {
		if (op == 'b')
			q.push_back();
		if (op == 'f')
			q.push_front();
		q.print(reel);
	};

	auto pop = [&] {
		if (op == 'b')
			q.pop_back();
		if (op == 'f')
			q.pop_front();
		q.print(reel);
	};

	auto advb = [&] {
		q.pop_back();
		q.push_front();
		q.print(reel);
	};

	auto advf = [&] {
		q.pop_front();
		q.push_back();
		q.print(reel);
	};

	auto deu = [&] {
		std::cout << "begin:" << q.begin << "/" << q.map(q.begin) <<"\n";
		std::cout << "end:  " << q.end << "/" << q.map(q.end) <<"\n";
		std::cout << "head: " << q.head << "/" << q.map(q.head) <<"\n";
		std::cout << "tail: " << q.tail << "/" << q.map(q.tail) <<"\n";
	};

	// v yes it's bad. That's intentional. v

	std::string commands;
	if (ar[1]) {
		commands = ar[1];
	} else {
		std::cin >> commands;
	}

	while (true) {

		for (char c : commands) {
			if (c == '+') push();
			if (c == '-') pop();

			if (c == 'b') op = c;
			if (c == 'f') op = c;

			if (c == '>') advf();
			if (c == '<') advb();
			if (c == 'd') deu();
			if (c == 'q') q = queue();
			if (c == 'm') q.pmap();
			if (c == 'p') q.print(reel);
			if (c == 'r') reel = !reel;

			if (c == '.') exit(1);

		}

		std::cin >> commands;
	}
}
