#pragma once
#include <string>
#include <fat/block.h>
namespace fat {
struct fs_attributes {
	std::string volume_name;

	unsigned total_sectors;
	unsigned sector_size;
	unsigned cluster_size;

	unsigned filesystem_info_sector;
	unsigned backup_boot_sector;

	struct {
		unsigned count;
		unsigned sector;
		unsigned size; // in sectors
	} fat;

	unsigned data_sector;
	block first_cluster;

	//! Calculate offset of a sector
	unsigned offset(unsigned sector) const { return sector * sector_size; }

	//! Calculate offset of a logical block
	uint64_t offset(block idx) const { return uint64_t(idx) * cluster_size; }

	//! Calculate offset of a cluster
	uint64_t cluster_pos(block idx) const
	{
		auto block = idx - first_cluster;
		return offset(data_sector) + offset(block);
	}
};
} // namespace fat
