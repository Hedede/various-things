#pragma once
#include <cstddef>
#include <cstdint>
namespace fat {
enum class block : uint32_t {};
constexpr block empty{0u};
constexpr block bad{0x0FFF'FFF7u};
constexpr block eof{0x0FFF'FFF8u};

constexpr bool operator< (block a, block b) { return uint32_t(a) <  uint32_t(b); }
constexpr bool operator>=(block a, block b) { return uint32_t(a) >= uint32_t(b); }
constexpr bool operator==(block a, block b) { return uint32_t(a) == uint32_t(b); }
constexpr block operator-(block a, block b)
{
	return block{ uint32_t(a) - uint32_t(b) };
}
constexpr block operator+(block a, block b)
{
	return block{ uint32_t(a) + uint32_t(b) };
}
} // namespace fat
