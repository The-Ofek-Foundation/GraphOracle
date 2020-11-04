#ifndef ORACLE_H
#define ORACLE_H

#include "graph.h"

#include <unordered_map>
#include <vector>

using namespace ogila;

class Oracle
{
public:
	Oracle(const Graph& g);

	unsigned get_distance(unsigned u, unsigned v) noexcept;
	std::vector<unsigned> get_shortest_path(unsigned u, unsigned v) noexcept;

	unsigned get_num_prophecies() const noexcept;

private:
	struct Prophecy
	{
		unsigned next_vertex, distance = INFINITY;
	};

	static const unsigned INFINITY = -1u;

	const Prophecy& get_prophecy(unsigned u, unsigned v) const noexcept;
	Prophecy& get_prophecy(unsigned u, unsigned v) noexcept;

	void find_shortest_paths(unsigned u) noexcept;
	std::vector<unsigned> build_shortest_path(unsigned u, unsigned v, bool reverse) const noexcept;

	const Graph& graph;
	std::vector<Prophecy> prophecies;
	unsigned num_prophecies;
};

#endif
