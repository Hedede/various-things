#include "entry.h"
#include <fat/structure-defs.h>
#include <aw/utility/unicode/convert.h>

namespace fat {
std::string lfn_name(lfn_entry const& entry)
{
	std::u16string ws;
	for (auto c : entry.name1) ws.push_back(c);
	for (auto c : entry.name2) ws.push_back(c);
	for (auto c : entry.name3) ws.push_back(c);
	ws = ws.substr( 0, ws.find(L'\0') );
	return aw::unicode::convert<std::string>(ws);
}

unsigned get_cluster(dir_entry const& entry)
{
	return entry.cluster_low + (entry.cluster_high << 16);
}

entry_type type_of(entry_data const& entry)
{
	auto tag    = static_cast<uint8_t>(entry[0]);
	auto attrib = static_cast<uint8_t>(entry[0xB]);

	if (tag == empty_tag)
		return entry_type::empty;
	if (tag == deleted_tag) {
		if (attrib == lfn_flags)
			return entry_type::deleted_lfn;
		if (attrib & dir_flag)
			return entry_type::deleted_dir;
		return entry_type::deleted;
	}

	if (attrib == lfn_flags)
		return entry_type::lfn;
	if (attrib & dir_flag)
		return entry_type::dir;
	return entry_type::file;
}

entry read_entry(entry_data const& data)
{
	auto type = type_of(data);
	using et = entry_type;
	switch(type) {
	case et::empty:
		return { type };
	case et::file:
	case et::dir:
	case et::deleted:
	case et::deleted_dir:
		{
			auto entry = from_bytes<dir_entry>(data);
			return {
				type,
				get_cluster(entry),
				cst(entry.short_name),
				0,
				entry.file_size
			};
		}
	case et::lfn:
		{
			auto lfn = from_bytes<lfn_entry>(data);
			return {
				type,
				lfn.sequence_number & lfn_index,
				lfn_name(lfn),
				(lfn.sequence_number & lfn_last) == lfn_last
			};
		}
	case et::deleted_lfn:
		{
			auto entry = from_bytes<lfn_entry>(data);
			return {
				type,
				0,
				lfn_name(entry)
			};
		}
	};
}
} // namespace fat
