#ifndef GRAPH_H
#define GRAPH_H

#include "PRNG.h"

#include <iostream>
#include <unordered_set>
#include <vector>

using namespace ogila;

struct Edge
{
	unsigned u, v;
};

class Graph
{
	// below:
	// V -> total number of vertices
	// E -> total number of edges
	// D -> largest degree of vertex
public:
	// O(V)
	Graph(unsigned num_vertices = 0u);

	// O(V)
	Graph(unsigned num_vertices, const std::vector<Edge>& edges);

	// O(1)
	// assumptions: v1, v2 : [1, V]
	void add_edge(unsigned v1, unsigned v2) noexcept;

	// O(1)
	// assumptions: v : [1, V]
	const std::unordered_set<unsigned>& get_adjacent_vertices(unsigned v) const noexcept;

	// O(1)
	// assumptions: v1 : [1, V]
	bool are_adjacent(unsigned v1, unsigned v2) const noexcept;

	// O(V + E)
	// assumptions: edge_probability : [0, 1]
	static Graph make_erdos_renyi_graph(unsigned num_vertices, double edge_probability, PRNG& prng = prng) noexcept;

	// O(V + E)
	// assumptions: min_degree >= 1
	static Graph make_barabasi_albert_graph(unsigned num_vertices, unsigned min_degree, PRNG& prng = prng) noexcept;

	// O(1)
	unsigned get_num_vertices() const noexcept;

	// O(V + E)
	void print(std::ostream& os) const noexcept;

private:
	std::vector<std::unordered_set<unsigned>> adjacent_vertices;

	static PRNG prng;
};

std::ostream& operator<<(std::ostream& os, const Graph& g);

#endif
