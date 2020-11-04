#include "graph.h"
#include "oracle.h"

#include "PRNG.h"

#include <cmath>
#include <iostream>
#include <stdio.h>
#include <vector>

namespace
{
	void print_path(const std::vector<unsigned> path)
	{
		for (unsigned i = 0u; i < path.size(); ++i)
		{
			std::cout << (i == 0u ? "" : " -> ") << path[i];
		}

		std::cout << '\n';
	}

	Graph make_erdos_renyi_graph(unsigned num_vertices, PRNG& gen)
	{
		double edge_probability = 2u * std::log(num_vertices) / num_vertices;
		return Graph::make_erdos_renyi_graph(num_vertices, edge_probability, gen);
	}

	Graph make_barabasi_albert_graph(unsigned num_vertices, PRNG& gen)
	{
		return Graph::make_barabasi_albert_graph(num_vertices, 4u, gen);
	}
}

int main()
{
	setlocale(LC_NUMERIC, "");

	Graph g(5, {{0, 1}, {0, 2}, {0, 3}, {2, 3}, {2, 4}});
	std::cout << g;

	Oracle o(g);

	printf("Small custom graph:\n");

	std::cout << o.get_distance(4, 1) << "\n";
	std::cout << o.get_distance(1, 4) << "\n";
	std::cout << o.get_distance(2, 1) << "\n";

	print_path(o.get_shortest_path(4, 1));

	std::cout << o.get_num_prophecies() << "\n";

	PRNG prng;

	Graph g2 = make_erdos_renyi_graph(10000, prng);
	Oracle o2(g2);

	printf("\nMedium Erdos Renyi graph:\n");

	std::cout << o2.get_distance(5, 13) << "\n";
	print_path(o2.get_shortest_path(5, 13));

	printf("\nMedium Barabasi Albert graph:\n");

	Graph g3 = make_barabasi_albert_graph(10000, prng);
	Oracle o3(g3);

	std::cout << o3.get_distance(5, 13) << "\n";
	print_path(o3.get_shortest_path(5, 13));

	return 0;
}
