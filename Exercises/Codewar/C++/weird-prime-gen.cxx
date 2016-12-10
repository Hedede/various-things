#include <vector>
#include <set>
#include <numeric>
#include <algorithm>
#include <iostream>
#include <cassert>
template<typename T>
T gcd(T a, T b)
{
	return b == 0 ? a : gcd(b, a % b);
}

struct sequence {
	struct iterator {
		using value_type = long long;
		using iterator_category = std::input_iterator_tag;

		using reference = value_type&;
		using pointer   = value_type*;
		using difference_type = long long;

		iterator() = default;

		long long operator*() const { return cur; }
		long long index() const { return n+1; }

		iterator& operator++()
		{
			assert(cur != 0);
			++n;
			cur += gcd(index(), cur);
			return *this;
		}

		iterator operator++(int)
		{
			iterator copy{*this};
			return ++*this, copy;
		}

		friend bool operator==(iterator a, iterator b) { return a.n == b.n; }
		friend bool operator!=(iterator a, iterator b) { return a.n != b.n; }

	protected:
		friend struct sequence;

		iterator(long long n) : cur{0}, n{n} {}

	private:
		long long cur = 7;
		long long n   = 0;
	};

	iterator begin() const
	{
		return {};
	}

	iterator end() const
	{
		return {n};
	}

	long long n;
};

struct ones_sequence {
	struct iterator : sequence::iterator {
		using value_type = long long;
		using iterator_category = std::input_iterator_tag;

		using reference = value_type&;
		using pointer   = value_type*;
		using difference_type = long long;

		iterator() = default;

		long long current() const { return sequence::iterator::operator*(); }
		long long previous() const { return prev; }

		long long operator*() const
		{
			if (prev == 0)
				return 1;
			return current() - prev;
		}

		iterator& operator++()
		{
			prev = current();
			sequence::iterator::operator++();
			return *this;
		}

		iterator operator++(int)
		{
			iterator copy{*this};
			return ++*this, copy;
		}

	protected:
		long long diff() const
		{
			return operator*();
		}

		void skip_ones()
		{
			while (diff() == 1)
				operator++();
		}

	private:
		friend struct ones_sequence;

		iterator(long long n) : sequence::iterator{n} {}

		long long prev = 0;
	};

	iterator begin() const
	{
		return {};
	}

	iterator end() const
	{
		return {n};
	}

	long long n;
};

auto begin(ones_sequence const& seq) { return seq.begin(); }
auto end(ones_sequence const& seq)   { return seq.end(); }

struct anover_sequence {
	struct iterator : ones_sequence::iterator {
		using value_type = double;
		using iterator_category = std::input_iterator_tag;

		using reference = value_type&;
		using pointer   = value_type*;
		using difference_type = long long;

		iterator()
		{
			skip_ones();
		}

		double operator*() const
		{
			return current() / index();
		}

		iterator& operator++()
		{
			skip_ones();
			++i;
			return *this;
		}

		iterator operator++(int)
		{
			iterator copy{*this};
			return ++*this, copy;
		}

		friend bool operator==(iterator a, iterator b) { return a.i == b.i; }
		friend bool operator!=(iterator a, iterator b) { return a.i != b.i; }

	protected:
		void skip_ones()
		{
			while (diff() == 1)
				ones_sequence::iterator::operator++();
		}

	private:
		friend struct anover_sequence;

		iterator(long long n) : i{n} {}
		long long i = 0;
	};

	iterator begin() const
	{
		return {};
	}

	iterator end() const
	{
		return {n};
	}

	long long n;
};

auto begin(anover_sequence const& seq) { return seq.begin(); }
auto end(anover_sequence const& seq)   { return seq.end(); }


class WeirdPrimeGen
{
public:
	static std::vector<long long> an(long long n)
	{
		return {sequence{n}.begin(), sequence{n}.end() };
	}

	static std::vector<long long> gn(long long n)
	{
		return { ones_sequence{n}.begin(), ones_sequence{n}.end() };
	}

	static auto p(long long n)
	{
		ones_sequence::iterator seq;
		std::vector<long long> primes;
		while (primes.size() < n) {
			long long term = *seq++;
			if (term == 1)
				continue;
			if (std::find(begin(primes), end(primes), term) != end(primes))
				continue;
			primes.push_back(term);
		}
		return primes;
	}

	static auto anOver(long long n)
	{
		return anover_sequence{n};
	}

public:
	static long long countOnes(long long n)
	{
		auto seq = ones_sequence{n};
		return std::count(begin(seq), end(seq), 1);
	}

	static long long maxPn(long long n)
	{
		auto seq = p(n);
		return *std::max_element(begin(seq), end(seq));
	}
	static int anOverAverage(long long n)
	{
		auto seq = anOver(n);
		auto sum = std::accumulate(begin(seq), end(seq), 0.0);
		std::cout << sum << '\n';
		return sum / n;
	}
};

int main()
{
	std::cout << "Test countOnes\n";
	std::cout << WeirdPrimeGen::countOnes(1) << '\n';
	std::cout << WeirdPrimeGen::countOnes(10) << '\n';
	std::cout << "Test maxPn\n";
	std::cout << WeirdPrimeGen::maxPn(1) << '\n';
	std::cout << WeirdPrimeGen::maxPn(5) << '\n';
	std::cout << "Test anOverAverage\n";
	std::cout << WeirdPrimeGen::anOverAverage(1) << '\n';
}

#if 0
class WeirdPrimeGen
{
public:
	static std::vector<long long> an(long long n)
	{
		static std::vector<long long> seq{ 7 };
		while (seq.size() < n) {
			long long a = seq.back();
			long long n = seq.size() + 1;
			seq.push_back(a + gcd( n, a ) );
		}
		return { begin(seq), begin(seq) + n };
	}

	static std::vector<long long> gn(long long n)
	{
		static std::vector<long long> seq;
		if (seq.size() < n) {
			seq = an(n);
			std::adjacent_difference(begin(seq), end(seq), begin(seq));
			seq[0] = 1;
		}
		return { begin(seq), begin(seq) + n };
	}

	static auto p(long long n)
	{
		std::vector<long long> seq;
		auto m = n*log(n+1);
		do {
			seq = gn(m);
			auto newend = std::remove(begin(seq), end(seq), 1);
			auto primes = std::set<long long>();
			newend = std::remove_if(begin(seq), newend,
			[&primes] (long long x) mutable {
				if (!primes.insert(x).second)
					return true;
				return false;
			});
			
			seq.erase(newend, end(seq));
			m *= m+1;
		} while(seq.size() < n);

		seq.resize(n);
		return seq;
	}

	static auto anOver(long long n)
	{
		sequence::iterator seq;
		ones_sequence::iterator seq1;
		std::vector<double> anover;
		for (; anover.size() < n; ++seq, ++seq1) {
			if (*seq1 == 1)
				continue;
			anover.push_back(double(*seq)/seq.index());
		}
	}

public:
	static long long countOnes(long long n)
	{
		auto seq = gn(n);
		return std::count(begin(seq), end(seq), 1);
	}
	static long long maxPn(long long n)
	{
		auto seq = p(n);
		return *std::max_element(begin(seq), end(seq));
	}
	static int anOverAverage(long long n)
	{
		return 0;
	}
};


int main()
{
	//std::cout << WeirdPrimeGen::maxPn(1) << '\n';
	std::cout << WeirdPrimeGen::maxPn(5) << '\n';

	auto seq = WeirdPrimeGen::gn(4000000);
	long long ones = 0;
	int prev = 0;
	for (int i = 1; i < 4000000; ++i) {
		int j = i-(ones += seq[i-1] == 1);
		if (j != prev) {
			prev = j;
			std::cout <<i << ':' << ' ' << j << ' ' << int(log2(i)*4) << '\n';
		}
	}
}
#endif
