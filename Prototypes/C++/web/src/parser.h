#include <aw/types/containers/flat_map.h>
#include <aw/types/string_view.h>
#include <aw/types/strip.h>
#include <aw/meta/conditional.h>
#include <functional>
#include <vector>
#include <iosfwd>

namespace aw {
namespace web {
struct string_sv_comp : std::less<std::string>, std::less<string_view> {
	using std::less<std::string>::operator();
	using std::less<string_view>::operator();

	bool operator()(std::string const& s, string_view sv) const
	{
		return string_view{s} < sv;
	}

	bool operator()(string_view sv, std::string const& s) const
	{
		return sv < string_view{s};
	}
};

template<typename Map, typename K>
auto get_maybe( Map& map, K&& key )
	-> copy_const<Map, typename Map::mapped_type>*
{
	using std::begin; using std::end;
	auto i = map.find( key );
	if (i != end(map))
		return &i->second;
	return nullptr;
}

struct context {
	template<typename T>
	using string_map = aw::flat_map<std::string, T, string_sv_comp>;
	using outputter = std::function<void(std::ostream& os)>;
	string_map<std::string> vars;
	string_map<outputter> funcs;

	std::string const* get_var(string_view varname) const
	{
		return get_maybe( vars, varname );
	}

	outputter const* get_func(string_view funcname) const
	{
		return get_maybe( funcs, funcname );
	}
};

struct html_template {
	html_template(std::string buf);

	struct block {
		string_view data;

		enum class category {
			static_content,
			variable,
			function
		} cat;
	};

	const std::string buffer;
	std::vector<block> blocks;

	void produce(std::ostream& os, context const& ctx);
};
} // namespace web
} // namespace aw
