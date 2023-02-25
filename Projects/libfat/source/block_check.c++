#include <fat/block_check.h>
namespace fat {

void block_checker::check_range( )
{
	for (auto& [i,block] : aw::ipairs(map.list)) {
		auto current = block(i);

		if (block == empty) continue;
		out.update_block( block );

		if (is_special(block)) continue;
		if (block > map.last()) {
			out.report_problem( outside_of_bounds{ current, block } );
			block = eof;
		}
	}
}

bool block_checker::check_block( block current, block prev )
{
	if (next == eof)
		return false;
	if (next == bad) {
		out.report_problem( bad_cluster{file, prev} );
		return false;
	}
	if (next == empty) {
		map[prev] = eof;
		out.report_problem( empty_cluster{file, prev} );
		out.update_block( current );
		return false;
	}
	return true;
}

bool block_checker::check_owner( block current, size_t file )
{
	auto owner = map.owner_of(current);
	if (owner != no_owner) {
		report_problem( intersecting_chains{file, owner, current} );
		return false;
	}
	return true;
}


void block_checker::check_chain( block& start, size_t file )
{
	if (start > map.last()) {
		report_problem( invalid_first_cluster{file, start} );
		start = empty;
		return;
	}

	if (!check_owner(start))
		return;

	block prev;
	block current = start

	do {
		out.update_block( current );
		map.set_owner( current, file );
		out.update_block( current );
		prev    = current;
		current = map.next(current);
	} while (check_block(current, prev) && check_owner(current, file));
}
} // namespace fat
