#include <iostream>
#include <fat/structure-defs.h>
#include <aw/utility/string/trim.h>
#include <aw/utility/ranges/ipairs.h>
#include "entry.h"
#include "filesystem.h"
#include "block_iterator.h"
#include "directory_iterator.h"
namespace fat {
void filesystem_impl::report_problem(problem kind)
{
	out.report_problem( kind );
}

void filesystem_impl::add_file(fat::file file)
{
	out.add_file(file);
	files.emplace_back(std::move(file));
	dirs.emplace_back();
}

bool filesystem_impl::check_block(block_map& map, size_t file, block prev, block next)
{
	if (next >= eof)
		return true;
	if (next == bad) {
		report_problem( bad_cluster{file, prev} );
		return false;
	}
	if (next == empty) {
		report_problem( empty_cluster{file, prev} );
		return false;
	}
	if (next > map.last()) {
		report_problem( outside_of_bounds{file, prev, next} );
		return false;
	}

	auto owner = map.owner_of(next);
	if (owner != no_owner) {
		report_problem( intersecting_chains{file, owner, next} );
		return false;
	}
	return true;
}

bool filesystem_impl::check_file_consistency(block_map& map, size_t idx)
{
	auto& file = files[idx];
	if (file.status != file_status::unknown)
		return file.status != file_status::error;

	block start = file.first_block;
	for (auto [prev, current] : block_iterator{map, start}) {
		out.update_block( current );
		if (!check_block(map, idx, prev, current)) {
			file.status = file_status::error;
			return false;
		}
		map.set_owner(current, idx);
		out.update_block( current );
	}

	file.status = file_status::correct;
	return true;
}

void filesystem_impl::read_vbr()
{
	volume_boot_record vbr;
	read_struct(io.read, vbr);

	info.volume_name = cst(vbr.volume_name);
	info.total_sectors = vbr.sectors_in_partition;
	info.first_cluster = block{vbr.root_dir_cluster};
	info.sector_size   = vbr.bytes_per_sector;
	info.cluster_size  = info.sector_size * vbr.sectors_per_cluster;

	info.fat.count   = vbr.number_of_fat_copies;
	info.fat.sector  = vbr.reserved_sectors;
	info.fat.size    = vbr.sectors_per_fat;

	info.data_sector = info.fat.sector + info.fat.size * vbr.number_of_fat_copies;

	info.backup_boot_sector     = vbr.backup_boot_sector;
	info.filesystem_info_sector = vbr.filesystem_info_sector;
}

void filesystem_impl::read_fat(unsigned idx)
{
	uint64_t fat_size   = info.offset(info.fat.size);
	uint64_t fat_offset = info.offset(info.fat.sector) + fat_size * idx;

	size_t num_entries = fat_size / sizeof(block);
	fat::block_map fat{info.first_cluster, num_entries};
	io.seek(fat_offset);
	io.read(fat.list.data(), fat_size);

	block_maps.emplace_back( std::move(fat) );
}

void filesystem_impl::read_root_dir(out_callbacks& _out)
{
	this->out = _out;
	fat::file root {
		"",
		"ROOT",
		file_type::directory,
		file_status::unknown,
		info.first_cluster,
		0
	};
	//TODO: PEG
	files.emplace_back(std::move(root));
	dirs.emplace_back();
	read_file(files.size() - 1);
}

void filesystem_impl::read_file(size_t idx)
{
	// TODO: both FATs
	auto& map = block_maps[0];

	check_file_consistency(map, idx);

	auto& file = files[idx];
	if (file.status != file_status::correct)
		return;
	if (is_dir(file))
		read_dir(idx);
}

static auto isspace = [] (char c) { return c == ' ';};

void filesystem_impl::read_dir(size_t idx)
{
	if (out.enter_directory)
		out.enter_directory(idx);

	// TODO: ugh, ugly
	auto& map = block_maps[0];
	auto& file = files[idx];
	auto iter = directory_iterator{ io, map, info, file.first_block };

	size_t new_entries_start = files.size();

	int lfn_number = -1;
	std::vector<std::string> lfns;

	for (auto data : iter) {
		auto entry = read_entry(data);
		if (out.add_dentry)
			out.add_dentry(entry);
		if (is_empty(entry.type))
			continue;

		if (entry.type == entry_type::lfn) {
			if (lfn_number == -1) {
				// TODO: check that it's last
				lfn_number = int(entry.index);
			} else {
				--lfn_number;
				// check entry.index == lfn_number;
			}
			lfns.push_back(entry.name);
			continue;
		}

		aw::string::rtrim(entry.name, isspace);
		if (aw::in(entry.name, ".", ".."))
			continue;

		std::string name;
		if (lfn_number != -1) {
			for (auto s : lfns)
				name = s + name;
			lfns.clear();
			lfn_number = -1;
		}

		auto determine_ft = [] (auto entry) {
			if (entry.type == entry_type::dir)
				return file_type::directory;
			return file_type::file;
		};
		fat::file file {
			name,
			entry.name,
			determine_ft(entry),
			file_status::unknown,
			block(entry.index),
			entry.size,
			idx
		};

		if (file.first_block == empty)
			file.status = file_status::empty;
		auto idx = files.size();
		add_file(std::move(file));
		check_file_consistency(map, idx);
	}

	size_t new_entries_end = files.size();

	auto& dir = dirs[idx];
	if (new_entries_start != new_entries_end) {
		dir.begin = new_entries_start;
		dir.end   = new_entries_end;
	}

	for (auto i = dir.begin; i < dir.end; ++i)
		read_file(i);
}

void filesystem_impl::mark_lost(block_map& map, block start)
{
	size_t count = 1;
	out.update_block( start );
	map.set_owner(start, lost_owner);
	for (auto [prev, current] : block_iterator{map, start}) {
		if (current == empty)
			break;
		out.update_block( current );
		map.set_owner(current, lost_owner);
		++count;
	}

	report_problem( lost_chain{ start, count} );
}

void filesystem_impl::find_lost(out_callbacks& out)
{
	auto& map = block_maps[0];
	for (auto [i, owner] : aw::ipairs( map.owner )) {
		auto cur = block(i);
		auto next = map.next(cur);
		//out.update_block( cur );
		if (cur < info.first_cluster)
			continue;
		if (next == empty || next == bad)
			continue;
		if (owner == no_owner)
			mark_lost( map, cur );
	}
}
} //namespace fat
