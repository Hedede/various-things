#include <tuple>
#include <utility>
#include <cstddef>

using namespace std;
template<size_t N>
struct vec {
	int v[N];
};


template<size_t N, class...Ts, size_t...M>
vec<sizeof...(M)> suqa1(tuple<Ts...> const& t, index_sequence<M...>)
{
	return { get<N+M>(t)... };
}

template<size_t M, size_t N>
struct mat {
	vec<M> v[N];
};

template<class...Ts, size_t... M, size_t...N>
mat<sizeof...(M), sizeof...(N)> suqa(tuple<Ts...> const& t, index_sequence<N...>, index_sequence<M...>)
{
	constexpr size_t S = sizeof...(N);
	index_sequence<(M*S)...> seq{};
	return { suqa1<N>( t, seq )... };
}

template<size_t M, size_t N>
struct matrix {
	mat<M,N> v;

	template<typename...Ts>
	matrix(Ts&&...ts)
		: v{ suqa(make_tuple(ts...), make_index_sequence<N>{}, make_index_sequence<M>{}) }
	{}

};

#include <iostream>
int main()
{
	matrix<4,2> m{
		1,2,
		3,4,
		5,6,
		7,8
	};

	std::cout << m.v.v[0].v[1] << '\n';
}
