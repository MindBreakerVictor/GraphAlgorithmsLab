#include "PCH.h"
#include "Tree.h"

Tree::Tree(uint32_t vertices) : UndirectedGraph(vertices)
{
	for (uint32_t i = 0; i < vertices - 1; i++)
	{
		uint32_t x, y;
		std::cin >> x >> y;
		_adjacencyList[x].push_back(y);
		_adjacencyList[y].push_back(x);
	}
}

Tree::Tree(std::ifstream& ifs)
{
	ifs >> _vertices;
	_adjacencyList.resize(_vertices);

	for (uint32_t i = 0; i < _vertices - 1; i++)
	{
		uint32_t x, y;
		ifs >> x >> y;
		_adjacencyList[x].push_back(y);
		_adjacencyList[y].push_back(x);
	}
}

uint32_t Tree::getDiameter() const
{
	srand((uint32_t)time(NULL));
	uint32_t vertex = rand() % _vertices;
	uint32_t firstLeaf = *breadthFirstSearch(vertex).rbegin();								// The last element discovered by BFS.
	std::vector<int> roadDistance = getRoadDistance(firstLeaf);								// Get the distance from firstLeaf to all vertices in the tree.
	uint32_t diameter = *std::max_element(roadDistance.cbegin(), roadDistance.cend()) + 1;	// Set the diameter to the highest value from roadDistance + 1.

	return diameter;
}

