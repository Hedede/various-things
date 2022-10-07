#include <type_traits>
#include <iterator>
#include <vector>
using namespace std;

template<typename T, typename = void>
constexpr bool has_iterator_traits = false;

template<typename T>
constexpr bool has_iterator_traits<
	T,
	void_t<
		typename std::iterator_traits<T>::value_type,
		typename std::iterator_traits<T>::difference_type,
		typename std::iterator_traits<T>::reference,
		typename std::iterator_traits<T>::pointer,
		typename std::iterator_traits<T>::iterator_category
	>
> = true;

template<typename T>
using increment = decltype( ++declval<T&>() );

template<typename T>
using dereference = decltype( *declval<T>() );

template<typename T, typename = void>
constexpr bool is_incrementable = false;

template<typename T>
constexpr bool is_incrementable< T, void_t<increment<T>> > {
	is_same< increment<T>, T&>::value
};


template<typename T, typename = void>
constexpr bool is_dereferencable = false;

template<typename T>
constexpr bool is_dereferencable< T, void_t< dereference<T> > > = true;




		//decltype( *declval<T>() ),
    
template<typename T>
constexpr bool is_iterator = has_iterator_traits<T> && is_incrementable<T> && is_dereferencable<T>;

#include <iostream>
int main()
{
	std::cout <<  is_iterator<std::vector<int>::iterator>;
	std::cout <<  is_iterator<int*>;
	std::cout <<  is_iterator<int>;
	std::cout <<  is_incrementable<int*>;
	std::cout <<  is_incrementable<int>;
	std::cout <<  is_incrementable<bool>;
}
