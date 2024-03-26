#pragma once
#include <string>
#include <aw/types/enum.h>
namespace fat {
template<typename... Ts>
static std::string str(Ts&&... v)
{
	auto to_string = [] (auto&& arg) {
		using T = std::remove_reference_t<decltype(arg)>;
		if constexpr( std::is_same_v<T, char> )
			return arg;
		if constexpr( std::is_enum_v<T> )
			return std::to_string( aw::underlying(arg) );
		else if constexpr( std::is_integral_v<T> )
			return std::to_string(arg);
		else
			return arg;
	};
	return (to_string(v) + ...);
}

template<size_t N>
std::string cst(char (&v)[N])
{
	return {v, N};
}
} // namespace fat
