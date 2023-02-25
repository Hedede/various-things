#pragma once
#include <aw/utility/ranges/ipairs.h>
#include <fat/callbacks.h>
#include <fat/problem.h>
#include <fat/block_map.h>
namespace fat {
struct block_checker {
	block_map& map;
	callbacks& out;

	void check_range();
	void check_chain( block& start, size_t file );
	void check_lost();

private:
	bool check_block( block current, block prev, size_t  );
	bool check_owner( block current, size_t file );
	void mark_lost( block start );
};
} // namespace fat
