#include "oracle.h"

#include "graph.h"

#include <algorithm>
#include <queue>
#include <unordered_map>
#include <vector>

using namespace ogila;

Oracle::Oracle(const Graph& g)
	: graph(g), prophecies(g.get_num_vertices() * g.get_num_vertices()), num_prophecies(0u)
{
	for (unsigned v = 0u; v < g.get_num_vertices(); ++v)
	{
		auto& prophecy = get_prophecy(v, v);
		prophecy.next_vertex = v;
		prophecy.distance = 0u;
	}
}

unsigned Oracle::get_distance(unsigned u, unsigned v) noexcept
{
	++num_prophecies;

	if (v < u)
	{
		unsigned temp = u;
		u = v;
		v = temp;
	}
	// u < v

	const auto& prophecy = get_prophecy(u, v);
	// printf("%d %d\n", u, v);
	// printf("%d %d\n", prophecy.distance, u * graph.get_num_vertices() + v);
	if (prophecy.distance == INFINITY)
	{
		find_shortest_paths(u);
	}

	return prophecy.distance;
}

std::vector<unsigned> Oracle::get_shortest_path(unsigned u, unsigned v) noexcept
{
	++num_prophecies;

	bool reverse = v < u;

	if (reverse)
	{
		unsigned temp = u;
		u = v;
		v = temp;
	}
	// u < v

	const auto& prophecy = get_prophecy(u, v);
	if (prophecy.distance == INFINITY)
	{
		find_shortest_paths(u);
	}

	return build_shortest_path(u, v, reverse);
}

namespace
{
	struct Vertex
	{
		unsigned v;
		unsigned distance;

		bool operator < (const Vertex& vertex) const noexcept
		{
			return distance < vertex.distance;
		}
	};
}

void Oracle::find_shortest_paths(unsigned u) noexcept
{
	std::priority_queue<Vertex> queue;
	queue.push({ u, 0u });

	while (!queue.empty())
	{
		Vertex vertex = queue.top();
		queue.pop();

		auto& prophecy = get_prophecy(u, vertex.v);
		if (prophecy.distance < vertex.distance)
		{
			continue;
		}

		prophecy.distance = vertex.distance;

		for (const auto& adjacent_vertex : graph.get_adjacent_vertices(vertex.v))
		{
			// printf("%d\n", adjacent_vertex);
			auto& other_prophecy = get_prophecy(u, adjacent_vertex);
			if (other_prophecy.distance > vertex.distance + 1u)
			{
				other_prophecy.distance = vertex.distance + 1u;
				other_prophecy.next_vertex = vertex.v;
				queue.push({ adjacent_vertex, vertex.distance + 1u });
			}
		}
	}
}

std::vector<unsigned> Oracle::build_shortest_path(unsigned u, unsigned v, bool reverse) const noexcept
{
	if (u == v)
	{
		return {};
	}

	Prophecy prophecy = get_prophecy(u, v);
	std::vector<unsigned> shortest_path(prophecy.distance);

	shortest_path[prophecy.distance - 1u] = reverse ? u : v;

	for (unsigned next_vertex = prophecy.next_vertex, i = 0u; i < prophecy.distance - 1u; next_vertex = get_prophecy(u, next_vertex).next_vertex, ++i)
	{
		shortest_path[reverse ? i : (prophecy.distance - 2u - i)] = next_vertex;
	}

	return shortest_path;
}

const Oracle::Prophecy& Oracle::get_prophecy(unsigned u, unsigned v) const noexcept
{
	return prophecies[u * graph.get_num_vertices() + v];
}

Oracle::Prophecy& Oracle::get_prophecy(unsigned u, unsigned v) noexcept
{
	return prophecies[u * graph.get_num_vertices() + v];
}

unsigned Oracle::get_num_prophecies() const noexcept
{
	return num_prophecies;
}
