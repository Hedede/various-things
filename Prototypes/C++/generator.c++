#include <type_traits>
#include <experimental/optional>

using namespace std;
using namespace std::experimental;

template <typename Generator>
struct sequence {
	using value_type = decltype( declval<Generator>()( ) );

	struct _internal {
		_internal( Generator _gen )
			: gen{ std::move(_gen) }, value{ gen( ) }
		{ }
		Generator  gen;
		value_type value;
	};
	optional<_internal> data;
	size_t index = 0;

	sequence(size_t end)
		: index{end}
	{}

	sequence(Generator gen)
		: data{ std::move(gen) }
	{}

	auto const& operator*() const
	{
		return data->value;
	}

	sequence& operator++()
	{
		data->value = data->gen();
	}

	bool is_end() const
	{
		return bool(data);
	}

	bool operator!=(sequence& other)
	{
		return index != other.index;
	}
};

template <typename Generator>
struct indexed_sequence {
	using value_type = decltype( declval<Generator>()( 0 ) );

	struct _internal {
		_internal( Generator _gen )
			: gen{ std::move(_gen) }, value{ gen( 0 ) }
		{ }
		Generator  gen;
		value_type value;
	};
	optional<_internal> data;
	size_t index = 0;

	indexed_sequence(size_t end)
		: index{end}
	{}

	indexed_sequence(Generator gen)
		: data{ std::move(gen) }
	{}

	auto const& operator*() const
	{
		return data->value;
	}

	indexed_sequence& operator++()
	{
		data->value = data->gen(++index);
	}

	bool is_end() const
	{
		return bool(data);
	}

	bool operator!=(indexed_sequence& other)
	{
		return index != other.index;
	}
};

#include <iostream>
#include <algorithm>
#include <numeric>
int main()
{
	auto gen = [x = 1] (size_t index) mutable { return x *= 2; };
	auto seq = indexed_sequence<decltype(gen)>( gen );
	std::cout << std::accumulate( seq, decltype(seq){20}, 0 ) << '\n';
}
