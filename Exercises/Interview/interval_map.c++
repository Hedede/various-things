bool docount = false;
int noper = 0;
int nmiss = 0;

#include <cassert>
#include <map>
template<typename K, typename V>
class interval_map {
public:
	V m_valBegin;
	std::map<K,V> m_map;
public:
	interval_map(V const& val)
	: m_valBegin(val)
	{}

	void assign( K const& keyBegin, K const& keyEnd, V const& val ) {
		++noper;
		docount = true;
		assign_impl( keyBegin, keyEnd, val );
		docount = false;
	}

	void assign_impl( K const& keyBegin, K const& keyEnd, V const& val ) {
		if (!( keyBegin < keyEnd )) {
			return;
		}

		auto beginIt = m_map.lower_bound(keyBegin);
		auto endIt   = m_map.upper_bound(keyEnd);

		if (endIt == m_map.begin()) {
			const auto& prevVal = m_valBegin;
			if (!(prevVal == val)) {
				auto hint = endIt;
				endIt = m_map.emplace_hint(endIt, keyEnd, prevVal);
				if (std::prev(hint) != endIt)
					++nmiss;
			}
		} else {
			const auto prevIt = std::prev(endIt);

			const auto& prevVal = prevIt->second;
			if (!(prevVal == val)) {
				const auto& prevKey = prevIt->first;
				const bool keysEqual = !(prevKey < keyEnd);
				if (keysEqual) {
					endIt = prevIt;
				} else {
					auto hint = endIt;
					endIt = m_map.emplace_hint(endIt, keyEnd, prevVal);
					if (std::prev(hint) != endIt)
						++nmiss;
				}
			}
		}


		{
			bool insertBegin = true;
			if (beginIt == m_map.begin()) {
				const auto& prevVal = m_valBegin;
				if (prevVal == val) {
					insertBegin = false;
				}
			} else {
				const auto prevIt = std::prev(beginIt);
				if (prevIt->second == val) {
					insertBegin = false;
				}
			}

			if (insertBegin) {
				const bool keysEqual = (beginIt != m_map.end()) && !(keyBegin < beginIt->first);
				if (keysEqual) {
					beginIt->second = val;
				} else {
					auto hint = beginIt;
					beginIt = m_map.emplace_hint(hint, keyBegin, val);
					if (std::prev(hint) != beginIt)
						++nmiss;
				}
				beginIt = std::next(beginIt);
			}
		}

		if (beginIt != endIt) {
			m_map.erase(beginIt, endIt);
		}
	}

	V const& operator[]( K const& key ) const {
		auto it=m_map.upper_bound(key);
		if(it==m_map.begin()) {
			return m_valBegin;
		} else {
			return (--it)->second;
		}
	}
};

#include <iostream>
#include <random>

int key_comps = 0;
int val_comps = 0;
int key_copies = 0;
int key_assign = 0;
int val_copies = 0;
int val_assign = 0;
int key_destru = 0;
int val_destru = 0;

struct Key {
	int v;
	Key (int v) :v(v) {}
	Key (const Key& other)
		: v(other.v)
	{
		if (docount)
			++key_copies;
	}
	~Key()
	{
		if (docount)
			++key_destru;
	}
	Key& operator=(const Key& other)
	{
		v = other.v;
		if (docount)
			++key_assign;
		return *this;
	}
	friend bool operator<(const Key& a, const Key& b)
	{
		if (docount)
			++key_comps;
		return a.v<b.v;
	}
	friend std::ostream& operator<<(std::ostream& o, Key k) { return (o << k.v); }
};
Key Prev(Key k) { return Key(k.v-1); }
Key Next(Key k) { return Key(k.v+1); }

struct Val {
	using T = char;
	T v;
	Val (T v) :v(v) {}
	Val (const Val& other)
		: v(other.v)
	{
		if (docount)
			++val_copies;
	}
	~Val()
	{
		if (docount)
			++val_destru;
	}
	Val& operator=(const Val& other)
	{
		v = other.v;
		if (docount)
			++val_assign;
		return *this;
	}
	friend bool operator==(const Val& a, const Val& b)
	{
		if (docount)
			++val_comps;
		return a.v==b.v;
	};
	friend std::ostream& operator<<(std::ostream& o, Val k) { return (o << k.v); }
};
template<class K, class V>
bool test_canon(interval_map<K,V> m)
{
	V pv = m.m_valBegin;
	for (auto& [k,v] : m.m_map)
	{
		if (v == pv)
			return false;
		pv = v;
	}
	return true;
}

template<class K, class V>
void printmap(interval_map<K,V> m, bool x)
{
	std::cout << ">>> the map: ";
	if (!test_canon(m)) {
		std::cout << "noncanon!";
	}
	std::cout << '\n';
	if (m.m_map.empty())
		return;
	V pv = m.m_valBegin;
	K pk = Prev(m.m_map.begin()->first);
	for (auto& [k,v] : m.m_map)
	{
		for (K i = pk; pk < k; pk = Next(pk))
		{
			if (x) std::cout << pk << " = " << pv << '\n';
			else std::cout << pv << '|';
		}
		pv = v;
	}
	if (x) std::cout << pk << " = " << pv << '\n';
	else std::cout << pv;
	if (!x) std::cout <<'\n';
}

size_t avgsize = 0;

#include <aw/utility/random/mersenne_twister.h>
auto mt = aw::create_mersenne_twister_engine();
void fuzz(size_t size, Val defVal, size_t niter = 1000, size_t nval = 1000)
{
	std::vector<Val> testvec(size, defVal);
	interval_map<Key, Val> m(defVal);

	std::uniform_int_distribution<int> dist(0, size);
	std::uniform_int_distribution<int> dist3(0, 1000);
	std::uniform_int_distribution<char> dist2('A', 'z');
	for (size_t i = 0; i < niter; ++i)
	{
		auto beg = i;
		auto end = std::min<int>(size,i+dist3(mt));
		auto val = dist2(mt);
		for (auto i = beg; i < end; ++i)
		{
			testvec[i] = val;
		}

		m.assign(beg, end, val);

		if (!test_canon(m)) {
			std::cout << "noncanon!" << '\n';
		}

		for (auto i = std::max<int>(0, beg-1), e = std::min<int>(size, end+1); i<e; ++i)
		{
			if (m[i] == testvec[i])
				continue;
			std::cout << "hren!" << '\n';
		}
	}

	avgsize = m.m_map.size();
}

int main(int argc, char**argv)
{
	size_t size = 1000;
	fuzz(size, 'X', 1000);
	std::cout << "noper:" << noper << ' '  << avgsize << ' ' << log2(size) << '\n';
	std::cout << "nmiss:" << nmiss << '\n';
	std::cout << "comps: " << key_comps << ' ' << val_comps  << '\n';
	std::cout << "assign: "<< key_assign << ' ' << val_assign << '\n';
	std::cout << "destru: "<< key_destru << ' ' << val_destru << '\n';
	std::cout << "copies: "<< key_copies << ' ' << val_copies << ' ' << '\n';
	std::cout << double(key_comps + val_comps + key_assign + val_assign + key_destru + val_destru + key_copies + val_copies) / noper << '\n';
	return 0;

	using namespace std::string_view_literals;
	bool b = (argc>1 && argv[1] == "1"sv);
		
	// 1. Insert into empty map
	// 2. Overlapping intervals:
	// - [  ( ]  )
	// - (  [ )  ]
	// - [ (   ) ]
	// - ( [   ] )
	// 3. Overwrite whole map
	// 4. Overwrite whole map with m_valBegin

	fuzz(10, 'X', 1000000);
	fuzz(100, 'X', 10000);
	fuzz(1000, 'X');
	fuzz(100000, 'X');
	fuzz(10000, 'X');

	std::vector<Val> v(50, 'X');
	interval_map<Key, Val> m('X');

	auto do_assign = [&] (int beg, int end, Val val) {
		m.assign(beg,end,val);
		for (auto j = beg; j < end; ++j)
		{
			int i = j + 25;
			v[i] = val;
		}

		for (auto i = 0, e = 50; i<e; ++i)
		{
			int j = i - 25;
			if (m[j] == v[i])
				continue;
			std::cout << "hren!" << '\n';
		}
		//printmap(m, b);
	};

	do_assign(1,6,'A');
	do_assign(0,1,'X');
	do_assign(-10,1,'X');
	do_assign(3,5,'B');
	do_assign(4,5,'C');
	do_assign(4,7,'C');
	do_assign(-1,1,'D');
	do_assign(-1,7,'D');
	do_assign(-2,8,'E');
	do_assign(10,20,'X');
	do_assign(-3,10,'X');
	do_assign(0,8,'X');
	do_assign(0,8,'A');
	do_assign(7,11,'B');
	do_assign(7,11,'F');
}

