#pragma once
#include <fat/callbacks.h>
#include <fat/attributes.h>
#include "block_iterator.h"
namespace fat {
struct directory_iterator {
	directory_iterator(io_callbacks& io, block_map& map, fs_attributes& info, block blk);

	auto begin() { return *this; }
	auto end()   { return eof; }

	std::array<std::byte, 32> operator*()
	{
		std::array<std::byte, 32> tmp;
		auto a = cluster.begin() + pos;
		auto b = a + 32;
		std::copy(a, b, tmp.begin());
		return tmp;
	}

	directory_iterator& operator++()
	{
		pos += 32;
		if (pos >= info.cluster_size) {
			pos = 0;
			++blk;
			read_next();
		}
		return *this;
	}

	bool operator!=(block term) const
	{
		return blk != term;
	}

	size_t get_pos() const
	{
		return info.cluster_pos(blk.current()) + pos;
	}

private:
	io_callbacks& io;
	fs_attributes& info;
	block_iterator blk;
	std::vector<std::byte> cluster;
	size_t pos = 0;

	//TODO: rename read_data
	void read_next()
	{
		io.seek( info.cluster_pos(blk.current()) );
		io.read( cluster.data(), cluster.size() );
	}
};
} // namespace fat
