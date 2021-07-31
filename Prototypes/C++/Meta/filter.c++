#include <aw/meta/list.h>

using namespace aw::meta;

template<bool pass>
struct zero_or_one {
	template<typename T>
	using type = list<T>;
};

template<>
struct zero_or_one<false> {
	template<typename T>
	using type = list<>;
};

template<typename T, typename... L>
struct concatenate
{
	using type = T;
};



template<typename... Ts, typename... Us, typename... L>
struct concatenate<list<Ts...>, list<Us...>, L...>
{
	using type = concatenate<list<Ts...,Us...>, L...>::type;
};


template<typename... Ts, typename... L>
struct concatenate<list<Ts...>, list<>, L...>
{
	using type = concatenate<list<Ts...>,L...>::type;
};


template<typename... Us, typename... L>
struct concatenate<list<>, list<Us...>, L...>
{
	using type = concatenate<list<Us...>,L...>::type;
};

template<typename... L>
struct concatenate<list<>, list<>, L...>
{
	using type = concatenate<list<>, L...>::type;
};

template<template<typename T> typename P, typename... Ts>
struct filter {
	using type = concatenate<typename zero_or_one<P<Ts>::value>::template type<Ts>...>::type;

};

template<typename T>
struct none {
	static constexpr bool value = false;
};

template<typename T>
struct all {
	static constexpr bool value = true;
};

using test1 = filter<none, int,int,int,int>::type;
using test2 = filter<all, int,int,int,int>::type;

#include <type_traits>
//static_assert(std::is_same_v<test, list<int,int,int,int>>);

#include <iostream>
template<typename T>
void prif()
{
	std::cout << __PRETTY_FUNCTION__ << '\n';
}

int main()
{
	prif<test1>();
	prif<test2>();
}
