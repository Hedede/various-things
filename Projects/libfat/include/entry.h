#pragma once
#include <array>
#include <string>
#include <cstddef>
namespace fat {
enum class entry_type {
	empty,
	file,
	dir,
	lfn,
	deleted,
	deleted_dir,
	deleted_lfn,
};
struct entry {
	entry_type type;
	unsigned index = 0;
	std::string name;
	unsigned flags = 0;
	unsigned size  = 0;
};

using entry_data = std::array<std::byte,32>;
} // namespace fat
