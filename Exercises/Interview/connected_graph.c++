#include <random>
#include <algorithm>
#include <aw/utility/random/mersenne_twister.h>
std::mt19937 rng = aw::create_mersenne_twister_engine(); // code doesn't matter

#include <vector>
using adj_list = std::vector<size_t>;
using graph = std::vector<adj_list>;

graph generate_graph( size_t num_verts, size_t num_edges )
{
	graph g(num_verts);
	std::uniform_int_distribution<unsigned> rnd_vert( 0, g.size() - 1 );
	for (size_t i = 0; i < num_edges; ++i) {
		// connect two random verts
		auto v1 = rnd_vert(rng);
		auto v2 = rnd_vert(rng);
		g[v1].push_back(v2);
		g[v2].push_back(v1);
	}
	return g;
}

size_t count_connected( graph const& g )
{
	std::vector<bool> visited(g.size(), false);

	size_t num_components = 0;

	// depth-first search
	auto mark_connected = [&g, &visited] (size_t v) {
		std::vector<size_t> stack;
		stack.push_back(v);

		while (!stack.empty()) {
			auto top = stack.back();
			stack.pop_back();

			// skip vertex if it is already visited
			if (visited[top])
				continue;

			// otherwise mark it as visited
			visited[top] = true;
			// and push adjacent verts to stack
			for (auto a : g[top])
				stack.push_back(a);
		}
	};

	for (size_t i = 0, e = g.size(); i < e; ++i) {
		if (visited[i])
			continue;
		mark_connected( i );
		++num_components;
	}
	return num_components;
}

#include <iostream>
int main()
{
	auto g = generate_graph( 10, 10 );
	std::cout << count_connected( g );
}
