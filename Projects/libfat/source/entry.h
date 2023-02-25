#pragma once
#include <fat/entry.h>
#include <aw/algorithm/in.h>

namespace fat {
constexpr uint8_t empty_tag = 0;
constexpr uint8_t deleted_tag = 0xE5;
constexpr uint8_t lfn_flags = 0xF;
constexpr uint8_t dir_flag  = 0x10;
constexpr unsigned lfn_index = 0x1F;
constexpr unsigned lfn_last  = 0x40;

entry_type type_of(entry_data const& entry);

inline bool is_empty(entry_type type)
{
	using et = entry_type;
	return aw::in(type, et::empty, et::deleted, et::deleted_dir, et::deleted_lfn);
}

entry read_entry(entry_data const& data);


} // namespace fat
