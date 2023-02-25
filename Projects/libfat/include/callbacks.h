#pragma once
#include <functional>
#include <fat/entry.h>
#include <fat/file.h>
#include <fat/problem.h>
#include <fat/block.h>

namespace fat {
struct io_callbacks {
	std::function<bool(void const*, size_t)> write;
	std::function<bool(void*, size_t)>       read;
	std::function<bool(size_t)>              seek;
	std::function<size_t()>                  tell;
	std::function<size_t()>                  size;
};

struct out_callbacks {
	std::function<void(file)>    add_file;
	std::function<void(size_t)>  enter_directory;
	std::function<void(size_t)>  select_file;
	std::function<void(entry)>   add_dentry;
	std::function<void(problem)> report_problem;
	std::function<void(block)>   update_block;
};
} // namespace fat
