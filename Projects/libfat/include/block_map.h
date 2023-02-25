#pragma once
#include <fat/block.h>
#include <vector>
namespace fat {
constexpr size_t no_owner   = -1;
constexpr size_t lost_owner = -2;
struct block_map {
	block_map(block start_block, size_t num_entries)
		: start_block{start_block},
		  list(num_entries),
		  owner(num_entries, no_owner),
		  error(num_entries, false)
	{}

	block next(block cur) const
	{
		auto index = uint32_t(cur);
		return list[index];
	}

	size_t size() const
	{
		return list.size();
	}

	block first() const
	{
		return start_block;
	}

	block last() const
	{
		return block(size()-1);
	}

	size_t owner_of(block idx) const
	{
		return owner[size_t(idx)];
	}

	void set_owner(block idx, size_t file)
	{
		owner[size_t(idx)] = file;
	}

	block start_block;
	std::vector<block>  list;
	std::vector<size_t> owner;
	std::vector<bool>   error;
};
} // namespace fat
