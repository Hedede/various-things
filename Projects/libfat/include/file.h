#pragma once
#include <vector>
#include <fat/block.h>
#include <fat/file_type.h>
namespace fat {
enum class file_status {
	unknown,
	empty,
	correct,
	error
};
struct file {
	std::string const& name() const
	{
		return full_name.empty() ? dos_name : full_name;
	}
	std::string full_name;
	std::string dos_name;
	//char dos_name[11];
	file_type type;
	file_status status;
	block first_block;
	size_t size;
	size_t parent_id;
};

struct directory {
	size_t begin = 0;
	size_t end   = 0;
};

constexpr bool is_dir(fat::file& file)
{
	return file.type == file_type::directory;
}
} // namespace fat
