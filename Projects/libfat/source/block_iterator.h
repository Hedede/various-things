#pragma once
#include <fat/callbacks.h>
#include <fat/block_map.h>
#include <utility>
namespace fat {
struct block_iterator {
	block_iterator(block_map& map, block start)
		: map{map}, _cur{start}
	{
	}

	block_iterator& begin() { return *this; }
	block           end()   { return eof;   }

	block prev() const
	{
		return _prev;
	}

	block current() const
	{
		return _cur;
	}

	block next() const
	{
		return map.next(_cur);
	}

	std::pair<block,block> operator*() const
	{
		return {_prev, _cur};
	}

	bool operator!=(block) const
	{
		return _cur < eof;
	}

	block_iterator& operator++()
	{
		_prev = current();
		_cur = next();
	}


private:
	block_map& map;
	block _cur;
	block _prev = empty;
};
} // namespace fat
