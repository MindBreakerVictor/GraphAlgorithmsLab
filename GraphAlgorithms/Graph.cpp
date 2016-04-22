#include "PCH.h"
#include "Graph.h"

Vector<uint32_t> Graph::breadthFirstSearch(uint32_t const& vertex) const
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
			if (!visited[_adjacencyList[element][i].first])
			{
				queue.push(_adjacencyList[element][i].first);
				visited[_adjacencyList[element][i].first] = true;
				connectedComponent.push_back(_adjacencyList[element][i].first);
			}
		queue.pop();
	}

	return connectedComponent;
}

Vector<uint32_t> Graph::depthFirstSearch(uint32_t const& vertex) const
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
			if (!visited[_adjacencyList[element][index].first])
				found = true;

		if (found)
		{
			index--;
			stack.push(_adjacencyList[element][index].first);
			visited[_adjacencyList[element][index].first] = true;
			connectedComponent.push_back(_adjacencyList[element][index].first);
		}
		else
			stack.pop();
	}

	return connectedComponent;
}

Vector<int> Graph::getRoadDistance(uint32_t const& vertex) const
{
	if (!isValidVertex(vertex))
		return Vector<int>();

	Vector<bool> visited(_vertices);
	Vector<int> roadDistance(_vertices, -1);
	PriorityQueue<Pair<uint32_t, int32_t>, Vector<Pair<uint32_t, int32_t>>, VerticesCostComparator<false>> pQueue;

	roadDistance[vertex] = 0;
	pQueue.push(std::make_pair(vertex, roadDistance[vertex]));

	while (!pQueue.empty())
	{
		uint32_t element = pQueue.top().first;

		pQueue.pop();

		if (visited[element])
			continue;

		visited[element] = true;

		for (Vector<Pair<uint32_t, int32_t>>::const_iterator itr = _adjacencyList[element].begin(); itr != _adjacencyList[element].end(); itr++)
		{
			uint32_t neighbour = itr->first;
			int32_t distance = itr->second;

			if (roadDistance[neighbour] > roadDistance[element] + distance)
			{
				roadDistance[neighbour] = roadDistance[element] + distance;
				pQueue.push(std::make_pair(neighbour, roadDistance[neighbour]));
			}
		}
	}

	return roadDistance;
}

bool Graph::isValidVertex(uint32_t const& vertex) const
{
	if (vertex > _vertices - 1)
		return false;

	return true;
}

std::ostream& operator<<(std::ostream& os, Graph const& graph)
{
	for (uint32_t i = 0; i < graph._adjacencyList.size(); i++)
	{
		os << i << " | ";
		for (uint32_t j = 0; j < graph._adjacencyList[i].size(); j++)
			if (!graph.isWeighted())
				os << graph._adjacencyList[i][j].first << ", ";
			else
				os << graph._adjacencyList[i][j].first << " - " << graph._adjacencyList[i][j].second << ", ";
		os << "\n";
	}

	return os;
}

std::ofstream& operator<<(std::ofstream& ofs, Graph const& graph)
{
	for (uint32_t i = 0; i < graph._adjacencyList.size(); i++)
	{
		ofs << i << " | ";
		for (uint32_t j = 0; j < graph._adjacencyList[i].size(); j++)
			if (!graph.isWeighted())
				ofs << graph._adjacencyList[i][j].first << ", ";
			else
				ofs << graph._adjacencyList[i][j].first << " - " << graph._adjacencyList[i][j].second << ", ";
		ofs << "\n";
	}

	return ofs;
}

