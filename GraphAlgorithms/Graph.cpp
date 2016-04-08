#include "PCH.h"
#include "Graph.h"

Vector<uint32_t> Graph::breadthFirstSearch(uint32_t vertex) const
{
	if (!isValidVertex(vertex))
		return Vector<uint32_t>();

	Vector<bool> visited(_vertices);
	Queue<uint32_t> queue;
	Vector<uint32_t> connectedComponent;

	queue.push(vertex);
	visited[vertex] = true;
	connectedComponent.push_back(vertex);

	while (!queue.empty())
	{
		uint32_t element = queue.front();

		for (uint32_t i = 0; i < _adjacencyList[element].size(); i++)
			if (!visited[_adjacencyList[element][i]])
			{
				connectedComponent.push_back(_adjacencyList[element][i]);
				queue.push(_adjacencyList[element][i]);
				visited[_adjacencyList[element][i]] = true;
			}
		queue.pop();
	}

	return connectedComponent;
}

Vector<uint32_t> Graph::depthFirstSearch(uint32_t vertex) const
{
	if (!isValidVertex(vertex))
		return Vector<uint32_t>();

	Vector<bool> visited(_vertices);
	Stack<uint32_t> stack;
	Vector<uint32_t> connectedComponent;

	stack.push(vertex);
	visited[vertex] = true;
	connectedComponent.push_back(vertex);

	while (!stack.empty())
	{
		uint32_t index, element = stack.top();
		bool found = false;

		for (index = 0; index < _adjacencyList[element].size() && !found; index++)
			if (!visited[_adjacencyList[element][index]])
				found = true;

		if (found)
		{
			index--;
			stack.push(_adjacencyList[element][index]);
			visited[_adjacencyList[element][index]] = true;
			connectedComponent.push_back(_adjacencyList[element][index]);
		}
		else
			stack.pop();
	}

	return connectedComponent;
}

Vector<int> Graph::getRoadDistance(uint32_t vertex) const
{
	if (!isValidVertex(vertex))
		return Vector<int>();

	Vector<bool> visited(_vertices);
	Queue<uint32_t> queue;
	Vector<int> roadDistance(_vertices, -1);

	queue.push(vertex);
	visited[vertex] = true;
	roadDistance[vertex] = 0;

	while (!queue.empty())
	{
		uint32_t element = queue.front();

		for (uint32_t i = 0; i < _adjacencyList[element].size(); i++)
			if (!visited[_adjacencyList[element][i]])
			{
				roadDistance[_adjacencyList[element][i]] = roadDistance[element] + 1;
				queue.push(_adjacencyList[element][i]);
				visited[_adjacencyList[element][i]] = true;
			}
		queue.pop();
	}

	return roadDistance;
}

bool Graph::isValidVertex(uint32_t vertex) const
{
	if (vertex < 0 || vertex > _vertices - 1)
		return false;

	return true;
}

std::ostream& operator<<(std::ostream& os, const Graph& graph)
{
	for (uint32_t i = 0; i < graph._adjacencyList.size(); i++)
	{
		os << i << " | ";
		for (uint32_t j = 0; j < graph._adjacencyList[i].size(); j++)
			os << graph._adjacencyList[i][j] << ", ";
		os << "\n";
	}

	return os;
}

std::ofstream& operator<<(std::ofstream& ofs, const Graph& graph)
{
	for (uint32_t i = 0; i < graph._adjacencyList.size(); i++)
	{
		ofs << i << " | ";
		for (uint32_t j = 0; j < graph._adjacencyList[i].size(); j++)
			ofs << (uint32_t)graph._adjacencyList[i][j] << ", ";
		ofs << "\n";
	}

	return ofs;
}

