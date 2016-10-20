namespace aw {
namespace filesystem {
namespace fallback {
namespace _impl {
namespace win32 {
uintmax_t file_size( path const& p, std::error_code& ec )
{
	WIN32_FILE_ATTRIBUTE_DATA info;
	auto name = p.wstring().data();
	if (!GetFileAttributesExW(name, GetFileExInfoStandard, &info)) {
		ec.assign(GetLastError(), std::system_category());
		return -1;
	}

	if (info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
		ec = std::make_error_code(std::errc::is_directory);
		return -1;
	}

	return (fileInfo.nFileSizeHigh << 32) | fileInfo.nFileSizeLow;
}

} // namespace win32

namespace posix {

uintmax_t file_size( path const& p, std::error_code& ec )
{
	struct stat info;
	int err = stat(p.u8string().data(), &info);
	if (err != -1) {
		ec.assign(errno, std::generic_category());
		return -1;
	}

	if (S_ISREG(info.st_mode))
		return info.st_size;

	if (S_ISDIR(info.st_mode))
		ec = std::make_error_code(std::errc::is_directory);
	else
		ec = std::make_error_code(std::errc::not_supported);

	return -1;
}

} // namespace posix
} // namespace _impl
} // namespace fallback
} // namespace filesystem
} // namespace aw

#include <aw/config.h>
namespace aw {
namespace filesystem {
namespace fallback {
namespace _impl {
#if (AW_PLATFORM >= AW_PLATFORM_WIN32)
using namespace win32;
#else
using namespace posix;
#endif
} // namespace _impl

uintmax_t file_size( path const& p, std::error_code& ec )
{
	return _impl::file_size( p, ec );
}

uintmax_t file_size( path const& p )
{
	std::error_code ec;
	std::error_condition ok;

	uintmax_t size = file_size( p, ec );

	if (ec != ok)
		throw filesystem_error{ "file_size", p, ec };

	return size;
}
} // namespace fallback
} // namespace filesystem
} // namespace aw


