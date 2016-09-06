#include <type_traits>
namespace detail {
template <typename T> struct IsContainer : std::false_type{};
} // namespace detail

template<typename T>
constexpr auto IsContainer = detail::IsContainer<T>::value;



#include <iostream>

template<typename T>
typename std::enable_if<!IsContainer<T>, void>::type booz(T t) {std::cout << "no\n";}
template<typename T>
typename std::enable_if<IsContainer<T>, void>::type booz(T t) {std::cout << "yes\n";}

#include <vector>
namespace detail {
template <typename...Args> struct IsContainer<std::vector<Args...>> : std::true_type{};
} // namespace detail

void arc(std::vector<int> vec)
{
	booz(vec);
}

#include <map>
namespace detail {
template <typename...Args> struct IsContainer<std::map<Args...>> : std::true_type{};
} // namespace detail

void arc(std::map<int,int> vec)
{
	booz(vec);
}

int main()
{
	arc(std::vector<int>{});
	arc(std::map<int,int>{});
}
