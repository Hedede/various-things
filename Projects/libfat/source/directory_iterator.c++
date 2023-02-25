#include "directory_iterator.h"

namespace fat {
directory_iterator::directory_iterator(io_callbacks& io, block_map& map, fs_attributes& info, block start)
	: io{io},
	  blk{map, start},
	  info{info},
	  cluster(info.cluster_size)
{
	read_next();
}
} // namesspace fat
