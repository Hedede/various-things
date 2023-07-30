#include <unordered_map>

enum class socket_handle : int {};

struct handle_hash {
	auto operator()(socket_handle const& p) const noexcept
	{
		std::hash<int> hasher;
		return hasher( int(p) );
	}
};

std::unordered_map<socket_handle, void*, handle_hash> sockets;
