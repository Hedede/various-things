#ifndef LIBFAT_STRUCTURE_H
#define LIBFAT_STRUCTURE_H
#include <cstring>
#include <cstddef>
#include <cstdint>

#include <aw/meta/pp/separators.h>
#include <aw/meta/pp/struct.h>

#include <aw/types/support/reinterpret.h>

//----------------------------------------
#define FAT_DEFINE_STRUCT_TIE(NAME,...)\
	constexpr auto tie() { \
		return std::tie( AW_LIST_MEMBER_NAMES( NAME,__VA_ARGS__ ) );\
	}

#define FAT_DEFINE_STRUCT(NAME,...) \
struct NAME {\
	AW_DEFINE_STRUCT_MEMBERS(NAME,__VA_ARGS__);\
	FAT_DEFINE_STRUCT_TIE(NAME,__VA_ARGS__);\
};

//----------------------------------------
namespace fat {
template<typename F, typename T>
constexpr decltype(auto) apply(F&& f, T&& s)
{
	return std::apply( std::forward<F>(f), s.tie() );
}

//----------------------------------------
template<typename F, typename T>
bool read_struct(F&& read_func, T& struc)
{
	auto read = [] (auto&& read_func, auto&& out) {
		return read_func( reinterpret_cast<void*>(&out), sizeof(out) );
	};
	auto read_visitor = [&] (auto&... args) {
		return (read(std::forward<F>(read_func), args) && ... );
	};
	return apply(read_visitor, struc);
}

//----------------------------------------
template<typename T>
T& copy_struct(void const* memory, T& struc)
{
	auto ptr = static_cast<std::byte const*>(memory);
	auto copy = [] (std::byte const*& ptr, auto& struc) {
		aw::copy_from_memory(struc, ptr);
		ptr += sizeof(struc);
	};
	auto copy_visitor = [&] (auto&... args) {
		(copy(ptr,args), ... );
	};
	apply(copy_visitor, struc);

}

//----------------------------------------
template<typename T, size_t N>
T from_bytes(std::array<std::byte,N> const& arr)
{
	T tmp;
	copy_struct(arr.data(), tmp);
	return tmp;
}

//----------------------------------------
template<size_t N>
std::string cst(char (&v)[N])
{
	return {v, N};
}
} // namespace fat
#endif//LIBFAT_STRUCTURE_H
