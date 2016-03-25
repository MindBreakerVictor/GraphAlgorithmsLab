#include "Graph.h"

bool Graph::isValidVertex(uint vertex) const
{
	if (vertex < 0 || vertex > vertices - 1)
		return false;

	return true;
}

std::ostream& operator<<(std::ostream& os, const Graph& graph)
{
	for (uint i = 0; i < graph.adjacencyList.size(); i++)
	{
		os << i << " | ";
		for (uint j = 0; j < graph.adjacencyList[i].size(); j++)
			os << graph.adjacencyList[i][j] << " ";
		os << "\n";
	}

	return os;
}

std::ofstream& operator<<(std::ofstream& ofs, const Graph& graph)
{
	for (uint i = 0; i < graph.adjacencyList.size(); i++)
	{
		ofs << i << " | ";
		for (uint j = 0; j < graph.adjacencyList[i].size(); j++)
			ofs << graph.adjacencyList[i][j] << " ";
		ofs << "\n";
	}

	return ofs;
}
