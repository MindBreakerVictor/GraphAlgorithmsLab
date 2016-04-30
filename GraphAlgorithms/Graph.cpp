#include "PCH.h"
#include "Graph.h"

uint32_t Graph::GetDegree(uint32_t const& vertex) const
{
	if (!IsValidVertex(vertex))
		return 0;

	return GetInDegree(vertex) + GetOutDegree(vertex);
}

uint32_t Graph::GetInDegree(uint32_t const& vertex) const
{
	if (!IsValidVertex(vertex))
		return 0;

	uint32_t count = 0;

	for (uint32_t i = 0; i < GetVertices(); ++i)
	{
		for (uint32_t j = 0; j < _adjacencyList[i].size(); ++j)
			if (_adjacencyList[i][j].first == vertex)
				++count;
	}

	return count;
}

uint32_t Graph::GetOutDegree(uint32_t const& vertex) const
{
	if (!IsValidVertex(vertex))
		return 0;

	return static_cast<uint32_t>(_adjacencyList[vertex].size());
}

uint32_t Graph::GetMinDegree() const
{
	if (!HasVertices() || !HasEdges() || GetVertices() == 1)
		return 0;

	uint32_t minDegree = UINT32_MAX;

	for (uint32_t i = 0; i < GetVertices(); ++i)
		if (GetDegree(i) < minDegree)
			minDegree = GetDegree(i);

	return minDegree;
}

uint32_t Graph::GetMaxDegree() const
{
	if (!HasVertices() || !HasEdges() || GetVertices() == 1)
		return 0;

	uint32_t maxDegree = 0;

	for (uint32_t i = 0; i < GetVertices(); ++i)
		if (GetDegree(i) > maxDegree)
			maxDegree = GetDegree(i);

	return maxDegree;
}

uint32_t Graph::GetMinInDegree() const
{
	if (!HasVertices() || !HasEdges() || GetVertices() == 1)
		return 0;

	uint32_t minInDegree = UINT32_MAX;

	for (uint32_t i = 0; i < GetVertices(); ++i)
		if (GetInDegree(i) < minInDegree)
			minInDegree = GetInDegree(i);

	return minInDegree;
}

uint32_t Graph::GetMinOutDegree() const
{
	if (!HasVertices() || !HasEdges() || GetVertices() == 1)
		return 0;

	uint32_t minOutDegree = UINT32_MAX;

	for (uint32_t i = 0; i < GetVertices(); ++i)
		if (GetOutDegree(i) < minOutDegree)
			minOutDegree = GetOutDegree(i);

	return minOutDegree;
}

uint32_t Graph::GetMaxInDegree() const
{
	if (!HasVertices() || !HasEdges() || GetVertices() == 1)
		return 0;

	uint32_t maxInDegree = 0;

	for (uint32_t i = 0; i < GetVertices(); ++i)
		if (GetInDegree(i) > maxInDegree)
			maxInDegree = GetInDegree(i);

	return maxInDegree;
}

uint32_t Graph::GetMaxOutDegree() const
{
	if (!HasVertices() || !HasEdges() || GetVertices() == 1)
		return 0;

	uint32_t maxOutDegree = 0;

	for (uint32_t i = 0; i < GetVertices(); ++i)
		if (GetOutDegree(i) > maxOutDegree)
			maxOutDegree = GetOutDegree(i);

	return maxOutDegree;
}

double Graph::GetDensity() const
{
	return (static_cast<double>(_edges) / static_cast<double>(GetVertices() * (GetVertices() - 1)));
}

Vector<uint32_t> Graph::BreadthFirstSearch(uint32_t const& vertex) const
{
	if (!IsValidVertex(vertex))
		return Vector<uint32_t>();

	Vector<bool> visited(GetVertices());
	Queue<uint32_t> queue;
	Vector<uint32_t> connectedComponent;

	queue.push(vertex);
	visited[vertex] = true;
	connectedComponent.push_back(vertex);

	while (!queue.empty())
	{
		uint32_t element = queue.front();

		for (uint32_t i = 0; i < _adjacencyList[element].size(); ++i)
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

Vector<uint32_t> Graph::DepthFirstSearch(uint32_t const& vertex) const
{
	if (!IsValidVertex(vertex))
		return Vector<uint32_t>();

	Vector<bool> visited(GetVertices());
	Stack<uint32_t> stack;
	Vector<uint32_t> connectedComponent;

	stack.push(vertex);
	visited[vertex] = true;
	connectedComponent.push_back(vertex);

	while (!stack.empty())
	{
		uint32_t index, element = stack.top();
		bool found = false;

		for (index = 0; index < _adjacencyList[element].size() && !found; ++index)
			if (!visited[_adjacencyList[element][index].first])
				found = true;

		if (found)
		{
			--index;
			stack.push(_adjacencyList[element][index].first);
			visited[_adjacencyList[element][index].first] = true;
			connectedComponent.push_back(_adjacencyList[element][index].first);
			continue;
		}

		stack.pop();
	}

	return connectedComponent;
}

Vector<int> Graph::GetRoadDistance(uint32_t const& vertex) const
{
	if (!IsValidVertex(vertex))
		return Vector<int>();

	Vector<bool> visited(GetVertices());
	Vector<int> roadDistance(GetVertices(), -1);
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

		for (AdjacencyListConstIterator itr = _adjacencyList[element].begin(); itr != _adjacencyList[element].end(); ++itr)
		{
			uint32_t neighbour = itr->first;
			int32_t distance = itr->second;

			if ((roadDistance[neighbour] > (roadDistance[element] + distance)) || (roadDistance[neighbour] < 0))
			{
				roadDistance[neighbour] = roadDistance[element] + distance;
				pQueue.push(std::make_pair(neighbour, roadDistance[neighbour]));
			}
		}
	}

	return roadDistance;
}

std::ostream& operator<<(std::ostream& os, Graph const& graph)
{
	for (uint32_t i = 0; i < graph._adjacencyList.size(); ++i)
	{
		os << i << " | ";

		for (uint32_t j = 0; j < graph._adjacencyList[i].size(); ++j)
			if (!graph.IsWeighted())
				os << graph._adjacencyList[i][j].first << ", ";
			else
				os << graph._adjacencyList[i][j].first << " - " << graph._adjacencyList[i][j].second << ", ";

		os << "\n";
	}

	return os;
}

std::ofstream& operator<<(std::ofstream& ofs, Graph const& graph)
{
	for (uint32_t i = 0; i < graph._adjacencyList.size(); ++i)
	{
		ofs << i << " | ";

		for (uint32_t j = 0; j < graph._adjacencyList[i].size(); ++j)
			if (!graph.IsWeighted())
				ofs << graph._adjacencyList[i][j].first << ", ";
			else
				ofs << graph._adjacencyList[i][j].first << " - " << graph._adjacencyList[i][j].second << ", ";

		ofs << "\n";
	}

	return ofs;
}

