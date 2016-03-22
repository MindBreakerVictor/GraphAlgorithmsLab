#include "DirectedGraph.h"

// -- Public members

DirectedGraph::DirectedGraph(std::ifstream& input)
{
	input >> vertices >> edges;
	graph.resize(vertices);

	for (uint i = 0; i < edges; i++)
	{
		uint x, y;
		input >> x >> y;
		graph[x].push_back(y);
	}
}

uint DirectedGraph::getDegree(uint vertex) const
{
	if (!isValidVertex(vertex))
		return 0;

	return getInDegree(vertex) + getOutDegree(vertex);
}

uint DirectedGraph::getInDegree(uint vertex) const
{
	if (!isValidVertex(vertex))
		return 0;

	uint count = 0;

	for (uint i = 0; i < graph.size(); i++)
	{
		for (uint j = 0; j < graph[i].size(); j++)
			if (graph[i][j] == vertex)
				count++;
	}

	return count;
}

uint DirectedGraph::getOutDegree(uint vertex) const
{
	if (!isValidVertex(vertex))
		return 0;

	return graph[vertex].size();
}

uint DirectedGraph::getMinDegree() const
{
	if (!vertices || vertices == 1 || !edges)
		return 0;

	uint minDegree = getDegree(0);

	for (unsigned int i = 1; i < vertices; i++)
		if (getDegree(i) < minDegree)
			minDegree = getDegree(i);

	return minDegree;
}

uint DirectedGraph::getMinInDegree() const
{
	if (!vertices || vertices == 1 || !edges)
		return 0;

	uint minInDegree = getInDegree(0);

	for (unsigned int i = 1; i < vertices; i++)
		if (getInDegree(i) < minInDegree)
			minInDegree = getInDegree(i);

	return minInDegree;
}

uint DirectedGraph::getMinOutDegree() const
{
	if (!vertices || vertices == 1 || !edges)
		return 0;

	uint minOutDegree = getOutDegree(0);

	for (unsigned int i = 1; i < vertices; i++)
		if (getOutDegree(i) < minOutDegree)
			minOutDegree = getOutDegree(i);

	return minOutDegree;
}

uint DirectedGraph::getMaxDegree() const
{
	if (!vertices || vertices == 1 || !edges)
		return 0;

	uint maxDegree = getDegree(0);

	for (uint i = 1; i < vertices; i++)
		if (getDegree(i) > maxDegree)
			maxDegree = getDegree(i);

	return maxDegree;
}

uint DirectedGraph::getMaxInDegree() const
{
	if (!vertices || vertices == 1 || !edges)
		return 0;

	uint maxInDegree = getInDegree(0);

	for (uint i = 1; i < vertices; i++)
		if (getInDegree(i) > maxInDegree)
			maxInDegree = getInDegree(i);

	return maxInDegree;
}

uint DirectedGraph::getMaxOutDegree() const
{
	if (!vertices || vertices == 1 || !edges)
		return 0;

	uint maxOutDegree = getOutDegree(0);

	for (uint i = 1; i < vertices; i++)
		if (getOutDegree(i) > maxOutDegree)
			maxOutDegree = getOutDegree(i);

	return maxOutDegree;
}

bool DirectedGraph::isComplete() const
{
	for (uint i = 0; i < vertices; i++)
	{
		if (graph[i].size() < vertices - 1)
			return false;

		std::vector<bool> visited(vertices, false);
		visited[i] = true;

		for (uint j = 0; j < graph[i].size(); j++)
			visited[graph[i][j]] = true;

		for (uint j = 0; j < visited.size(); j++)
			if (!visited[j])
				return false;
	}

	return true;
}

bool DirectedGraph::isRegular() const
{
	for (uint i = 1; i < vertices; i++)
		if (getInDegree(i) != getInDegree(i - 1) || getOutDegree(i) != getOutDegree(i - 1))
			return false;

	return true;
}

bool DirectedGraph::isStronglyConnected() const
{
	for (uint i = 0; i < vertices; i++)
	{
		std::vector<int> roadDistance = getRoadDistance(i);

		for (uint j = 0; j < roadDistance.size(); j++)
			if (roadDistance[j] == -1)
				return false;
	}

	return true;
}

std::stack<uint> DirectedGraph::getTopologicalSort() const
{
	std::stack<uint> topSort;
	std::vector<bool> visited(vertices, false);

	for (uint vertex = 0; vertex < vertices; vertex++)
		if (!visited[vertex])
			topologicalSort(vertex, visited, topSort);

	return topSort;
}

std::vector<uint> DirectedGraph::breadthFirstSearch(uint vertex) const
{
	if (!isValidVertex(vertex))
		return std::vector<uint>();

	std::vector<bool> visited(vertices);
	std::queue<uint> queue;
	std::vector<uint> connectedComponent;

	queue.push(vertex);
	visited[vertex] = true;
	connectedComponent.push_back(vertex);

	while (!queue.empty())
	{
		uint element = queue.front();

		for (uint i = 0; i < getOutDegree(element); i++)
			if (!visited[graph[element][i]])
			{
				connectedComponent.push_back(graph[element][i]);
				queue.push(graph[element][i]);
				visited[graph[element][i]] = true;
			}
		queue.pop();
	}

	return connectedComponent;
}

std::vector<uint> DirectedGraph::depthFirstSearch(uint vertex) const
{
	if (!isValidVertex(vertex))
		return std::vector<uint>();

	std::vector<bool> visited(vertices);
	std::stack<uint> stack;
	std::vector<uint> connectedComponent;

	stack.push(vertex);
	visited[vertex] = true;
	connectedComponent.push_back(vertex);

	while (!stack.empty())
	{
		uint index, element = stack.top();
		bool found = false;

		for (index = 0; index < getOutDegree(element) && !found; index++)
			if (!visited[graph[element][index]])
				found = true;

		if (found)
		{
			index--;
			stack.push(graph[element][index]);
			visited[graph[element][index]] = true;
			connectedComponent.push_back(graph[element][index]);
		}
		else
			stack.pop();
	}

	return connectedComponent;
}

std::vector<int> DirectedGraph::getRoadDistance(uint vertex) const
{
	if (!isValidVertex(vertex))
		return std::vector<int>();

	std::vector<bool> visited(vertices);
	std::queue<uint> queue;
	std::vector<int> roadDistance(vertices, -1);

	queue.push(vertex);
	visited[vertex] = true;
	roadDistance[vertex] = 0;

	while (!queue.empty())
	{
		uint element = queue.front();

		for (uint i = 0; i < getOutDegree(element); i++)
			if (!visited[graph[element][i]])
			{
				roadDistance[graph[element][i]] = roadDistance[element] + 1;
				queue.push(graph[element][i]);
				visited[graph[element][i]] = true;
			}
		queue.pop();
	}

	return roadDistance;
}

std::vector<std::vector<bool>> DirectedGraph::getRoadMatrix() const
{
	std::vector<std::vector<bool>> roadMatrix;

	for (uint i = 0; i < vertices; i++)
	{
		std::vector<int> roadDistance = getRoadDistance(i);

		for (uint j = 0; j < roadDistance.size(); j++)
			if (roadDistance[j] != -1)
				roadMatrix[i][j] = true;
	}

	return roadMatrix;
}

// -- End of public members

// -- Operator overloading

DirectedGraph& DirectedGraph::operator=(const DirectedGraph& source)
{
	if (this == &source)
		return *this;

	vertices = source.vertices;
	edges = source.edges;
	graph = source.graph;

	return *this;
}

DirectedGraph operator+(const DirectedGraph& graphOne, const DirectedGraph& graphTwo)
{
	if (graphOne.vertices != graphTwo.vertices || graphOne.vertices == 0)
		return DirectedGraph();

	DirectedGraph sumGraph;
	sumGraph.vertices = graphOne.vertices;
	sumGraph.graph.resize(sumGraph.vertices);

	for (uint i = 0; i < sumGraph.vertices; i++)
	{
		for (uint j = 0; j < graphOne.graph[i].size(); j++)
			sumGraph.graph[i].push_back(graphOne.graph[i][j]);

		for (uint j = 0; j < graphTwo.graph[i].size(); j++)
		{
			bool found = false;

			for (uint k = 0; k < sumGraph.graph[i].size(); k++)
				if (sumGraph.graph[i][k] == graphTwo.graph[i][j])
					found = true;

			if (!found)
				sumGraph.graph[i].push_back(graphTwo.graph[i][j]);
		}
	}

	return sumGraph;
}

DirectedGraph operator-(const DirectedGraph& graphOne, const DirectedGraph& graphTwo)
{
	// TODO: better way to handle this
	if (graphOne.vertices != graphTwo.vertices || graphOne.vertices == 0)
		return DirectedGraph();

	DirectedGraph difGraph;
	difGraph.vertices = graphOne.vertices;
	difGraph.graph.resize(difGraph.vertices);

	for (uint i = 0; i < graphOne.graph.size(); i++)
	{
		for (uint j = 0; j < graphOne.graph[i].size(); j++)
		{
			bool found = false;
			for (uint k = 0; k < graphTwo.graph[i].size(); k++)
				if (graphOne.graph[i][j] == graphTwo.graph[i][k])
					found = true;

			if (!found)
				difGraph.graph[i].push_back(graphOne.graph[i][j]);
		}
	}

	return difGraph;
}

bool operator==(const DirectedGraph& graphOne, const DirectedGraph& graphTwo)
{
	if (graphOne.vertices != graphTwo.vertices || graphOne.edges != graphTwo.edges)
		return false;

	if ((graphOne + graphTwo).edges != graphOne.edges)
		return false;

	return true;
}

// -- End of operator overloading

// -- Private members

void DirectedGraph::topologicalSort(uint vertex, std::vector<bool>& visited, std::stack<uint>& topSort) const
{
	visited[vertex] = true;

	for (uint i = 0; i < graph[vertex].size(); i++)
		if (!visited[graph[vertex][i]])
			topologicalSort(graph[vertex][i], visited, topSort);

	topSort.push(vertex);
}

// -- End of private members
