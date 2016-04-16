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
	if (!_vertices)
		return 0;

	srand((uint32_t)time(NULL));
	uint32_t vertex = rand() % _vertices;
	uint32_t firstLeaf = *breadthFirstSearch(vertex).rbegin();								// The last element discovered by BFS.
	Vector<int> roadDistance = getRoadDistance(firstLeaf);									// Get the distance from firstLeaf to all vertices in the tree.
	uint32_t diameter = *std::max_element(roadDistance.cbegin(), roadDistance.cend()) + 1;	// Set the diameter to the highest value from roadDistance + 1.

	return diameter;
}

Vector<uint32_t> Tree::getCenter() const
{
	if (!_vertices)
		return Vector<uint32_t>();

	Vector<uint32_t> center;

	// Get the chain ends.
	srand((uint32_t)time(NULL));
	uint32_t vertex = rand() % _vertices;
	uint32_t firstLeaf = *breadthFirstSearch(vertex).rbegin();
	uint32_t secondLeaf = *breadthFirstSearch(firstLeaf).rbegin();

	// DFS
	Stack<uint32_t> stack;
	Vector<bool> visited(_vertices);

	stack.push(firstLeaf);
	visited[firstLeaf] = true;

	while (!stack.empty())
	{
		uint32_t element = stack.top();

		if (element == secondLeaf)
		{
			uint32_t stackSize = stack.size();

			for (uint32_t i = ((stackSize & 1) ? 0 : 1); i < (stackSize >> 1); i++)
				stack.pop();

			for (uint8_t i = (stackSize & 1); i < 2; i++)
			{
				center.push_back(stack.top());
				stack.pop();
			}

			break;
		}

		uint32_t index;
		bool found = false;

		for (index = 0; index < _adjacencyList[element].size() && !found; index++)
			if (!visited[_adjacencyList[element][index]])
				found = true;

		if (found)
		{
			index--;
			stack.push(_adjacencyList[element][index]);
			visited[_adjacencyList[element][index]] = true;
		}
		else
			stack.pop();
	}

	return center;
}

