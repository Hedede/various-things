#ifndef FAT_STRUCTURE_DEFS_H
#define FAT_STRUCTURE_DEFS_H
#include <fat/structure.h>

namespace fat {
FAT_DEFINE_STRUCT(volume_boot_record,      \
	(std::byte[3], jump_code),         \
	(char[8], vendor),                 \
	(int16_t, bytes_per_sector),       \
	(int8_t,  sectors_per_cluster),    \
	(int16_t, reserved_sectors),       \
	(uint8_t, number_of_fat_copies),   \
	(int16_t, root_dir_entries),       \
	(int16_t, num_small_sectors),      \
	(uint8_t, media_descriptor),       \
	(int16_t, sectors_per_fat_16),     \
	(int16_t, sectors_per_track),      \
	(int16_t, number_of_heads),        \
	(int32_t, hidden_sectors),         \
	(int32_t, sectors_in_partition),   \
	(int32_t, sectors_per_fat),        \
	(int16_t, flags),                  \
	(int16_t, version),                \
	(uint32_t, root_dir_cluster),      \
	(int16_t, filesystem_info_sector), \
	(int16_t, backup_boot_sector),     \
	(std::byte[12], reserved),         \
	(uint8_t,  logical_drive_number),  \
	(int8_t,   unused),                \
	(int8_t,   extended_signature),    \
	(uint32_t, serial_number),         \
	(char[11], volume_name),           \
	(char[8],  fat_name),              \
	(std::byte[420], executable_code), \
	(char[2], boot_record_signature)   \
);

FAT_DEFINE_STRUCT(filesystem_info_record,   \
	(char[4],  fs_info_signature),      \
	(std::byte[480], reserved1),        \
	(char[4],  fs_info_signature2),     \
	(int32_t,  num_free_clusters),      \
	(int32_t,  most_recent_cluster),    \
	(std::byte[12], reserved2),         \
	(std::byte[2],  unknown),           \
	(char[2],  boot_record_signature2)  \
);

FAT_DEFINE_STRUCT(dir_entry,        \
	(char[8], short_name),      \
	(char[3], extension),       \
	(uint8_t, attributes),      \
	(uint8_t, user_attributes), \
	(uint8_t, create_time_ms),  \
	(uint16_t, create_time),    \
	(uint16_t, create_date),    \
	(uint16_t, access_date),    \
	(uint16_t, cluster_high),  /* or ext attrib */ \
	(uint16_t, modify_time),    \
	(uint16_t, modify_date),    \
	(uint16_t, cluster_low),    \
	(uint32_t, file_size)       \
);

FAT_DEFINE_STRUCT(lfn_entry,        \
	(uint8_t, sequence_number), \
	(char16_t[5], name1),       \
	(uint8_t, attributes),      \
	(uint8_t, entry_type),      \
	(uint8_t, name_checksum),   \
	(char16_t[6], name2),       \
	(uint16_t, cluster),        \
	(char16_t[2], name3)        \
);
} // namespace fat
#endif//FAT_STRUCTURE_DEFS_H
