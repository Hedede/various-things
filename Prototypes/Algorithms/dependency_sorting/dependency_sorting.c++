#include <algorithm>
#include <atomic>
#include <vector>
#include <string>

using type_id = size_t;

struct type {
	std::string name;
	type_id id = -1;
	std::vector<type_id> dependencies;
	std::vector<type_id> dependants;
};


static std::atomic<type_id> counter = 0;

template<typename T>
type_id type_id_of()
{
	static const std::atomic<type_id> id = counter++;
	return id;
}

static std::vector<type> types;

template<typename T, typename...Deps>
void register_type()
{
	type_id current_id = type_id_of<T>();

	if (types.size() <= current_id)
		types.resize(current_id + 1);

	types[current_id].id = current_id;
	types[current_id].name = T::type_name;
	constexpr auto dependency_count = sizeof...(Deps);

	if constexpr(dependency_count > 0)
	{
		std::array<type_id, dependency_count> dependency_ids = { type_id_of<Deps>()... };

		types[current_id].dependencies.assign(dependency_ids.begin(), dependency_ids.end());

		auto max_dep = *std::max_element( dependency_ids.begin(), dependency_ids.end() );
		if (types.size() <= max_dep)
			types.resize(max_dep + 1);

		for (const auto& id : dependency_ids)
		{
			types[id].dependants.push_back(current_id);
		}
	}
}


struct A{ static constexpr auto type_name = "A";};
struct B{ static constexpr auto type_name = "B";};
struct C{ static constexpr auto type_name = "C";};
struct D{ static constexpr auto type_name = "D";};
struct E{ static constexpr auto type_name = "E";};
struct F{ static constexpr auto type_name = "F";};
struct G{ static constexpr auto type_name = "G";};
struct H{ static constexpr auto type_name = "H";};
struct I{ static constexpr auto type_name = "I";};
struct J{ static constexpr auto type_name = "J";};
struct K{ static constexpr auto type_name = "K";};

#include <aw/types/containers/queue.h>
#include <iostream>

int main()
{
	register_type<B, A>();
	register_type<F, E>();
	register_type<A>();
	register_type<D, B, C>();
	register_type<C, B, A>();
	register_type<E, A, B>();
	register_type<H, C, G>();
	register_type<G>();
	register_type<J, F, K>();
	register_type<I, G, J>();
	register_type<K, A, I>();


	std::vector<type_id> output;
	std::vector<bool> is_loaded(types.size());
	aw::queue<type_id> queue;


	auto add_to_output = [&] (type const& t)
	{
		output.push_back(t.id);
		is_loaded[t.id] = true;

		for (const auto& d : t.dependants)
			queue.push_back(d);
	};

	for (const auto& t : types) {
		if (t.dependencies.size() == 0)
			add_to_output(t);
	}

	while (!queue.empty()) {
		auto id = queue.front();
		std::cout << '[' << types[id].name << ']';
		queue.pop_front();

		for (const auto& t : queue)
			std::cout << '(' << types[t].name << ')';
		std::cout << '\n';

		if (is_loaded[id])
			continue;

		const auto& t = types[id];

		bool deps_loaded = true;
		for (const auto& depid : t.dependencies) {
			if (!is_loaded[depid]) {
				deps_loaded = false;
				break;
			}
		}

		if (deps_loaded)
			add_to_output(t);
	}

	for (const auto& tid : output)
		std::cout << '(' << types[tid].name << ')';
	std::cout << '\n';
}
