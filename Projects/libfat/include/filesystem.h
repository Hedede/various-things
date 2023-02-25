#pragma once
#include <fat/callbacks.h>
#include <fat/file.h>
#include <fat/block_map.h>
#include <fat/problem.h>
#include <fat/attributes.h>
#include <aw/types/array_view.h>
#include <memory>
namespace fat {
struct filesystem_impl;
struct filesystem_model {
	filesystem_model( io_callbacks io );
	~filesystem_model();

	aw::array_view<block_map> block_maps() const;

	fs_attributes const& attributes() const;
	void scan_filesystem(block_map const& map, out_callbacks out);

        aw::array_view<fat::file> files() const;

private:
	std::unique_ptr<filesystem_impl> _impl;
};
} // namespace fat
