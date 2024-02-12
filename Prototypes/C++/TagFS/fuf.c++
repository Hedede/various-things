#define FUSE_USE_VERSION 30

#include <fuse.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#include <iostream>
#include <fstream>
#include <string_view>
namespace tagfs {
std::ifstream proto;
size_t length;
static int getattr( const char *path, struct stat *st )
{
	st->st_uid = getuid();
	st->st_gid = getgid();
	st->st_atime = time( NULL );
	st->st_mtime = time( NULL );

	if ( strcmp( path, "/" ) == 0 )
	{
		st->st_mode = S_IFDIR | 0755;
		st->st_nlink = 5;
	}
	else
	{
		st->st_mode = S_IFREG | 0644;
		st->st_nlink = 1;
		st->st_size = length;
	}

	return 0;
}

static int readdir( const char *path, void *buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi )
{
	filler( buffer, ".", NULL, 0 );
	filler( buffer, "..", NULL, 0 );

	if ( strcmp( path, "/" ) == 0 )
	{
		filler( buffer, "1", NULL, 0 );
		filler( buffer, "2", NULL, 0 );
		filler( buffer, "3", NULL, 0 );
		filler( buffer, "4", NULL, 0 );
		filler( buffer, "5", NULL, 0 );
	}

	return 0;
}

static int read( const char *path, char *buffer, size_t size, off_t offset, struct fuse_file_info *fi )
{
	using namespace std::string_view_literals;
	std::cerr << "read(" << path << ", " << offset << ", " << size <<  ");\n";
	if ( path == "/"sv )
		return -1;

	proto.clear();
	proto.seekg(offset);
	std::cerr << "pos: " << proto.tellg() << '\n';
	proto.read( buffer, size-1 );
	std::cout << buffer << '\n';
	auto cnt = proto.gcount();
	buffer[cnt] = 0;
	std::cerr << "cnt: " << cnt << '\n';
	return cnt;
}
} // namespace tagfs

#include <vector>
int main( int argc, char *argv[] )
{
	static struct fuse_operations operations;
	operations.getattr = tagfs::getattr;
	operations.readdir = tagfs::readdir;
	operations.read    = tagfs::read;
	tagfs::proto.open( argv[1] );
	if (!tagfs::proto.is_open())
		std::cerr << "cantopen \"" << argv[1] << "\"\n";

	using namespace tagfs;
	proto.ignore( std::numeric_limits<std::streamsize>::max() );
	length = proto.gcount();
	proto.clear();
	proto.seekg( 0, std::ios_base::beg );

	std::vector<char*> args;
	args.insert( args.end(), argv[0] );
	args.insert( args.end(), argv+2, argv + argc );

	return fuse_main( args.size(), args.data(), &operations, NULL );
}
