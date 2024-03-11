#include <algorithm>
#include <iterator>
#include <vector>
template <typename BaseIterator>
struct dereference_iterator
{

	using iterator_category = typename std::iterator_traits<BaseIterator>::iterator_category;
	using difference_type   = typename std::iterator_traits<BaseIterator>::difference_type;


	using pointer    = typename BaseIterator::value_type;
	using reference  = decltype(*std::declval<pointer>());
	using value_type = std::remove_reference_t<reference>;

	BaseIterator base;

	dereference_iterator() = default;

	dereference_iterator(BaseIterator iter)
		: base(iter)
	{
	}

	dereference_iterator operator++()
	{
		++base;
		return *this;
	}

	dereference_iterator operator++(int)
	{
		dereference_iterator copy = *this;
		++base;
		return copy;
	}

	dereference_iterator operator--()
	{
		--base;
		return *this;
	}

	dereference_iterator operator--(int)
	{
		dereference_iterator copy = *this;
		--base;
		return copy;
	}

	dereference_iterator& operator+=(int i)
	{
		base += i;
		return *this;
	}

	dereference_iterator operator+(int i)
	{
		dereference_iterator copy = *this;
		copy += i;
		return copy;
	}
	dereference_iterator& operator-=(int i)
	{
		base -= i;
		return *this;
	}

	dereference_iterator operator-(int i)
	{
		dereference_iterator copy = *this;
		copy -= i;
		return copy;
	}
	auto operator*() -> reference
	{
		return **base;
	}

	auto operator->() -> pointer
	{
		return *base;
	}
	auto operator*() const -> reference
	{
		return **base;
	}

	auto operator->() const -> pointer
	{
		return *base;
	}

	friend difference_type operator-(dereference_iterator first, dereference_iterator second)
	{
		return first.base - second.base;
	}

	friend bool operator==(dereference_iterator first, dereference_iterator second)
	{
		return first.base == second.base;
	}

	friend bool operator!=(dereference_iterator first, dereference_iterator second)
	{
		return first.base != second.base;
	}
};

template <typename Iterator>
auto deref_iterator(Iterator iter) -> dereference_iterator<Iterator>
{
	return { iter };
}
    template<typename _Tp>
      using with_ref = _Tp&;

    template<typename _Tp>
      concept can_reference = requires { typename with_ref<_Tp>; };

    template<typename _Tp>
      concept dereferenceable = requires(_Tp& __t)
	{
	  { *__t } -> can_reference;
	};


template <typename Container>
void dedupe_pointers(Container& c)
{
	using pointer = typename Container::pointer;

	auto temp = std::vector<pointer>(c.size());
	for (size_t i = 0; i < temp.size(); ++i)
		temp[i] = &c[i];

	auto begin1 = deref_iterator(begin(temp));
	auto end1   = deref_iterator(end(temp));

	// sort pointers based on pointed-to values
	std::stable_sort(begin1, end1);

	auto new_end = std::unique(begin1, end1);

	// sort pointers back to the original order
	std::stable_sort(begin1.base, new_end.base);

	// move unique elements to a new container and replace the original
	auto begin2 = std::make_move_iterator(begin1);
	auto end2 = std::make_move_iterator(new_end);

	c = Container(begin2, end2);
}

template<typename Ptr>
struct ptrpair {
	Ptr p;
	size_t i;
	
	auto& operator*() const
	{
		return *p;
	}
};

template <typename Container>
void dedupe_pointers_copy(Container& c)
{
	using pointer = typename Container::pointer;

	auto temp = std::vector<ptrpair<pointer>>(c.size());
	for (size_t i = 0; i < temp.size(); ++i)
		temp[i] = ptrpair<pointer>{ &c[i], i };

	auto begin1 = deref_iterator(begin(temp));
	auto end1   = deref_iterator(end(temp));

	// sort pointers based on pointed-to values
	std::stable_sort(begin1, end1);

	auto new_end = std::unique(begin1, end1);

	Container temp2(std::distance(begin1, new_end));
	for (auto it = begin1; it != new_end; ++it)
	{
		temp2[it.base->i] = std::move(*it.base->p);
	}

	c = std::move(temp2);
}
#include <set>
template <typename Container>
void dedupe_set(Container& c)
{
	std::set<typename Container::value_type> vals;

	auto new_end = std::remove_if(begin(c), end(c), [&] (const auto& v) {
		auto r = vals.insert(v);
		return !r.second;
	});

	c.erase(new_end, end(c));
}
#include <unordered_set>
template <typename Container>
void dedupe_uset(Container& c)
{
	std::unordered_set<typename Container::value_type> vals;

	auto new_end = std::remove_if(begin(c), end(c), [&vals](const auto& v) {
			return !vals.insert(v).second;
	});

	c.erase(new_end, end(c));
}
template <typename Container>
void dedupe_set_no(Container& c)
{
	std::set vals(begin(c), end(c));
	c = Container(begin(vals), end(vals));
}
template <typename Container>
void dedupe_uset_no(Container& c)
{
	auto beg_it = std::make_move_iterator(begin(c));
	auto end_it = std::make_move_iterator(end(c));
	std::unordered_set vals(beg_it, end_it);
	{
		auto beg_it = std::make_move_iterator(begin(vals));
		auto end_it = std::make_move_iterator(end(vals));
		c = Container(beg_it, end_it);
	}
}
template <typename Container>
void dedupe_copy(Container& c)
{
	Container copy;
	copy.reserve(c.size());

	for (const auto& v : c)
	{
		if (std::find(begin(copy), end(copy), v) == end(copy))
			copy.push_back(v);
	}

	c = copy;
}
template <typename Container>
void dedupe_copy_set(Container& c)
{
	std::set<typename Container::value_type> vals;
	Container copy;
	copy.reserve(c.size());

	for (const auto& v : c)
	{
		if (!vals.contains(v)) {
			copy.push_back(v);
			vals.insert(v);
		}
	}

	c = copy;
}
template <typename Container>
void dedupe_copy_set_lb(Container& c)
{
	std::set<typename Container::value_type> vals;
	Container copy;
	copy.reserve(c.size());

	for (const auto& v : c)
	{
		auto it = vals.lower_bound(v);
		if (it == end(vals) || v < *it) {
			copy.push_back(v);
			vals.insert(it, v);
		}
	}

	c = copy;
}
template <typename Container>
void dedupe_copy_uset(Container& c)
{
	std::unordered_set<typename Container::value_type> vals;
	Container copy;
	copy.reserve(c.size());

	for (auto& v : c)
	{
		if (vals.find(v) == end(vals)) {
			vals.insert(v);
			copy.push_back(std::move(v));
		}
	}

	c = copy;
}

template <typename Container>
void dedupe_copy_uset_pre(Container& c)
{
	std::unordered_set<typename Container::value_type> vals;
	Container copy(c.size());

	size_t i = 0;
	for (auto& v : c)
	{
		if (vals.find(v) == end(vals)) {
			vals.insert(v);
			copy[i++] = std::move(v);
		}
	}
	copy.resize(i);
#if 0
	auto b = copy.begin();
	for (const auto& v : c)
	{
		if (vals.find(v) == end(vals)) {
			*b++ = v;
			vals.insert(v);
		}
	}

	copy.erase(b, end(copy));
#endif

	c = copy;
}


template <typename Container>
void dedupe_baseline(Container& c)
{
	std::stable_sort(begin(c), end(c));
	c.erase(std::unique(begin(c), end(c)), end(c));
}
#include <aw/utility/random/mersenne_twister.h>
#include <iostream>
#include <chrono>
#include <cassert>


template<typename T>
struct rgen;

template<>
struct rgen<int> {
	std::mt19937 rnd;
	std::uniform_int_distribution<size_t> d1;
	size_t c;

	rgen(size_t c, size_t in)
		:rnd(aw::create_mersenne_twister_engine()),
		 d1(1, in), c(c)
	{
	}

	int start() { return 0; }
	int next(int v) {
		if (d1(rnd) <= c)
			++v;
		return v;
	}

};

template<>
struct rgen<std::string> {
	std::mt19937 rnd;
	std::uniform_int_distribution<size_t> d1;
	size_t c;

	rgen(size_t c, size_t in)
		:rnd(aw::create_mersenne_twister_engine()),
		 d1(1, in), c(c)
	{
	}

	std::string start() { return std::string(50, 'A'); }
	std::string next(std::string v) {
		if (d1(rnd) <= c) {
			if (v.back() >= 'z')
				std::reverse(begin(v), end(v));
			v[v.size() - 1]++;
			std::next_permutation(begin(v), end(v));
		}
		return v;
	}

};

template<typename T>
void test(size_t size, size_t chance = 3, size_t in = 10)
{
	using namespace std::chrono;
	rgen<T> r(chance, in);

	auto j = r.start();
	std::vector<T> v(size);
	for (size_t i = 0; i < v.size(); ++i)
	{
		v[i] = j;
		j = r.next(j);
	}

	auto max = std::unordered_set(begin(v), end(v)).size();
	std::shuffle(begin(v), end(v), r.rnd);

	std::cout << "size: " << size << " / unique: " << max << '\n';

	auto print_time = [=] (auto s, auto t1, auto t2)
	{
		std::cout << s;
		if (size < 1000)
			std::cout << duration<double, std::nano>(t2 - t1).count() << "ns\n";
		else if (size < 100000)
			std::cout << duration<double, std::micro>(t2 - t1).count() << "μs\n";
		else
			std::cout << duration<double, std::milli>(t2 - t1).count() << "ms\n";
	};

	auto test = [=] (std::string title, void(*f)(std::vector<T>&))
	{
		auto t1 = steady_clock::now();
		for (int i = 0; i < 3; ++i) {
			auto v2 = v;
			f(v2);
		}
		auto t2 = steady_clock::now();
		//print_time("preheat:", t1, t2);
		for (int i = 0; i < 3; ++i)
		{
			auto v2 = v;
			auto t1 = steady_clock::now();
			f(v2);
			auto t2 = steady_clock::now();

			print_time(i == 0 ? title : std::string(title.size(), ' '), t1, t2);
		}
	};

	test("baseline:   ", dedupe_baseline);
	//test("set_no:     ", dedupe_set_no);
	test("uset_no:    ", dedupe_uset_no);
	test("pointer:    ", dedupe_pointers);
	//test("pointer2:   ", dedupe_pointers_copy);
	//test("uset:       ", dedupe_uset);
	test("copy_uset:  ", dedupe_copy_uset);
	if (size < 1000) {
		test("set:        ", dedupe_set);
		test("copy_set:   ", dedupe_copy_set_lb);
		test("copy:       ", dedupe_copy);
	}
}

int main()
{
	std::cout.imbue(std::locale(""));
	//std::cout << "==== ints ====\n";
	//test<int>(10);
	//test<int>(100);
	//test<int>(1000);
	//test<int>(10000);
	//test<int>(100000);
	//test<int>(1000000);
	//test<int>(1000000, 9, 10);
	//test<int>(1000000, 999, 1000);

	std::cout << "==== strings ====\n";
	test<std::string>(10, 1, 20);
	test<std::string>(10, 1, 10);
	test<std::string>(10, 4, 10);
	test<std::string>(10, 9, 10);
	test<std::string>(100, 1, 100);
	test<std::string>(100, 10, 100);
	test<std::string>(100, 50, 100);
	test<std::string>(100, 99, 100);
	test<std::string>(1000, 1, 10);
	test<std::string>(1000, 9, 10);
	test<std::string>(1000, 99, 100);
	test<std::string>(1000, 999, 1000);
	//test<std::string>(1000);
	//test<std::string>(10000);
	//test<std::string>(100000);
	//test<std::string>(1000000);
	//test<std::string>(1000000, 9, 10);
	test<std::string>(1000000, 999, 1000);
}
