#include "filesystem.h"
namespace fat {
filesystem_model::filesystem_model( io_callbacks io )
	: _impl{ new filesystem_impl{std::move(io)} }
{
	_impl->read_vbr();
	_impl->read_fats();
}

filesystem_model::~filesystem_model() = default;

fs_attributes const& filesystem_model::attributes() const
{
	return _impl->info;
}

auto filesystem_model::block_maps() const -> aw::array_view<block_map>
{
	return _impl->block_maps;
}


void filesystem_model::scan_filesystem(block_map const& map, out_callbacks out)
{
	_impl->read_root_dir(out);
	_impl->find_lost(out);
}

auto filesystem_model::files() const -> aw::array_view<fat::file>
{
	return _impl->files;
}
} //namespace fat
