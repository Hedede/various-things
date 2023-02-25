#include <fat/filesystem.h>
#include <fat/file.h>
#include <initializer_list>
namespace fat {
struct filesystem_impl {
	io_callbacks io;
	out_callbacks out;

	fs_attributes info;

	std::vector<file>      files;
	std::vector<directory> dirs;

	std::vector<block_map> block_maps;


	void add_file(fat::file file);

	void read_vbr();
	void read_fats()
	{
		for (auto i = 0u; i < info.fat.count; ++i)
			read_fat(i);
	}
	void read_fat(unsigned idx);
	void read_root_dir(out_callbacks& out);
	void find_lost(out_callbacks& out);

	void read_file(size_t idx);
	void read_dir(size_t idx);


	void report_problem(problem);
	bool check_block(block_map& map, size_t file, block cur, block next);
	bool check_file_consistency(block_map& map, size_t file);
	void mark_lost(block_map& map, block start);
};
} //namespace fat
