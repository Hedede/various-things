#include <cassert>
template <typename T, typename S = T>
struct Range {
	Range(T begin, S end)
		: first(begin), last(end)
	{ }

	struct iterator {
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using reference  = T&;
		using pointer    = T*;
		using iterator_category = std::input_iterator_tag;

		iterator(value_type v)
			: value(v)
		{ }

		reference operator*()
		{
			return value;
		}

		pointer operator->()
		{
			return &value;
		}

		iterator& operator++()
		{
			++value;
			return *this;
		}

		iterator& operator--()
		{
			--value;
			return *this;
		}

		bool operator==(iterator const& other)
		{
			return value == other.value;
		}

		bool operator!=(iterator const& other)
		{
			return value != other.value;
		}

	private:
		value_type value;
	};

	using reverse_iterator = std::reverse_iterator<iterator>;

	iterator begin()
	{
		return iterator(first);
	}

	iterator end()
	{
		return iterator(last);
	}

	reverse_iterator rbegin()
	{
		return reverse_iterator{end()};
	}

	reverse_iterator rend()
	{
		return reverse_iterator{begin()};
	}

private:
	T first;
	S last;
};

template<typename T, typename S>
Range<T,S> range(T a, S z)
{
	assert(a <= z);
	return {a, z};
}

template<typename T>
Range<T> range(T end)
{
	return {T{}, end};
}



template <typename Base>
struct ipairs_iterator {
	ipairs_iterator(Base const& iter)
		: iter{iter}
	{}

	using value_type = typename std::iterator_traits<Base>::value_type;
	using reference  = typename std::iterator_traits<Base>::reference;

	std::pair<size_t,reference> operator*()
	{
		return {idx,*iter};
	}

	ipairs_iterator& operator++()
	{
		++iter, ++idx;
		return *this;
	}

	bool operator!=(Base const& it)
	{
		return iter != it;
	}

	size_t idx = 0;
	Base iter;
};

// TODO: starting index?
template <typename Range>
struct ipairs {
	ipairs(Range&& range)
		: range(range)
	{}

	auto begin()
	{
		return ipairs_iterator{ std::begin(range) };
	}

	auto end()
	{
		return std::end(range);
	}

	Range&& range;

	using iterator = decltype( std::declval<ipairs>().begin() );
};

template<typename Range>
ipairs(Range&& range) -> ipairs<Range>;
