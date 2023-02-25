#pragma once
#include <variant>
#include <fat/block.h>
namespace fat {
struct empty_cluster {
	size_t file;
	block  pos;
};

struct bad_cluster {
	size_t file;
	block  pos;
};

struct outside_of_bounds {
	size_t file;
	block  pos;
	block  value;
};

struct intersecting_chains {
	size_t file1;
	size_t file2;
	block  pos;
};

struct lost_chain {
	block pos;
	size_t count;
};

using problem = std::variant<
	empty_cluster,
	bad_cluster,
	outside_of_bounds,
	intersecting_chains,
	lost_chain
>;
} // namespace fat
