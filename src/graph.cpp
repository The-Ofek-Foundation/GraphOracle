#include "graph.h"

#include "PRNG.h"

#include <cmath>
#include <iostream>
#include <unordered_set>
#include <vector>

using namespace ogila;

Graph::Graph(unsigned num_vertices)
	: adjacent_vertices(num_vertices, std::unordered_set<unsigned>{})
{
}

Graph::Graph(unsigned num_vertices, const std::vector<Edge>& edges)
	: Graph(num_vertices)
{
	for (const Edge& edge : edges)
	{
		add_edge(edge.u, edge.v);
	}
}

void Graph::add_edge(unsigned v1, unsigned v2) noexcept
{
	adjacent_vertices[v1].emplace(v2);
	adjacent_vertices[v2].emplace(v1);
}

const std::unordered_set<unsigned>& Graph::get_adjacent_vertices(unsigned v) const noexcept
{
	return adjacent_vertices[v];
}

bool Graph::are_adjacent(unsigned v1, unsigned v2) const noexcept
{
	return adjacent_vertices[v1].count(v2) == 1u;
}

Graph Graph::make_erdos_renyi_graph(unsigned num_vertices, double edge_probability, PRNG& prng) noexcept
{
	Graph graph(num_vertices);

	double log_r_factor = 1.0 / std::log(1.0 - edge_probability);

	int v = 1, w = -1;

	while (v < static_cast<int>(num_vertices))
	{
		double r = prng.get_random_double(1.0);
		w += 1 + std::log(r) * log_r_factor;

		while (w >= v && v < static_cast<int>(num_vertices))
		{
			w -= v;
			++v;
		}

		if (v < static_cast<int>(num_vertices))
		{
			graph.add_edge(v, w);
		}
	}

	return graph;
}

Graph Graph::make_barabasi_albert_graph(unsigned num_vertices, unsigned min_degree, PRNG& prng) noexcept
{
	Graph graph(num_vertices);


	std::vector<unsigned> edges;
	edges.reserve(2u * num_vertices * min_degree);

	unsigned current_edge_index = 0u;

	for (unsigned v = 0u; v < num_vertices; ++v)
	{
		for (unsigned i = 0u; i < min_degree; ++i, current_edge_index += 2u)
		{
			edges.emplace_back(v);

			unsigned r = prng.get_random_number(current_edge_index);
			edges.emplace_back(edges[r]);
		}
	}


	for (unsigned i = 0u; i < edges.size(); i += 2u)
	{
		if (edges[i] == edges[i + 1u])
		{
			// avoid adding self-edges
			continue;
		}

		graph.add_edge(edges[i], edges[i + 1u]);
	}

	return graph;
}

PRNG Graph::prng = PRNG{};


unsigned Graph::get_num_vertices() const noexcept
{
	return adjacent_vertices.size();
}

void Graph::print(std::ostream& os) const noexcept
{
	for (unsigned v = 0u; v < get_num_vertices(); ++v)
	{
		os << v << "\t->";

		bool initial = true;

		for (unsigned adjacent_vertex : adjacent_vertices[v])
		{
			os << (initial ? "" : ",") << " " << adjacent_vertex;
		}
		os << '\n';
	}
}

std::ostream& operator<<(std::ostream& os, const Graph& g)
{
	g.print(os);
	return os;
}
