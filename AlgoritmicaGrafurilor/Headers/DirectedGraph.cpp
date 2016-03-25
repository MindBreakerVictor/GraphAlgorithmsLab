#include "DirectedGraph.h"

// -- Public members

DirectedGraph::DirectedGraph(std::ifstream& input)
{
	input >> vertices >> edges;
	adjacencyList.resize(vertices);

	for (uint i = 0; i < edges; i++)
	{
		uint x, y;
		input >> x >> y;
		adjacencyList[x].push_back(y);
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

	for (uint i = 0; i < adjacencyList.size(); i++)
	{
		for (uint j = 0; j < adjacencyList[i].size(); j++)
			if (adjacencyList[i][j] == vertex)
				count++;
	}

	return count;
}

uint DirectedGraph::getOutDegree(uint vertex) const
{
	if (!isValidVertex(vertex))
		return 0;

	return adjacencyList[vertex].size();
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
		if (adjacencyList[i].size() < vertices - 1)
			return false;

		std::vector<bool> visited(vertices, false);
		visited[i] = true;

		for (uint j = 0; j < adjacencyList[i].size(); j++)
			visited[adjacencyList[i][j]] = true;

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
			if (!visited[adjacencyList[element][i]])
			{
				connectedComponent.push_back(adjacencyList[element][i]);
				queue.push(adjacencyList[element][i]);
				visited[adjacencyList[element][i]] = true;
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
			if (!visited[adjacencyList[element][index]])
				found = true;

		if (found)
		{
			index--;
			stack.push(adjacencyList[element][index]);
			visited[adjacencyList[element][index]] = true;
			connectedComponent.push_back(adjacencyList[element][index]);
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
			if (!visited[adjacencyList[element][i]])
			{
				roadDistance[adjacencyList[element][i]] = roadDistance[element] + 1;
				queue.push(adjacencyList[element][i]);
				visited[adjacencyList[element][i]] = true;
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
	adjacencyList = source.adjacencyList;

	return *this;
}

DirectedGraph DirectedGraph::operator+(const DirectedGraph& source) const
{
	if (this->vertices != source.vertices || this->vertices == 0)
		return DirectedGraph();

	DirectedGraph sumGraph;
	sumGraph.vertices = this->vertices;
	sumGraph.adjacencyList.resize(sumGraph.vertices);

	for (uint i = 0; i < sumGraph.vertices; i++)
	{
		for (uint j = 0; j < this->adjacencyList[i].size(); j++)
			sumGraph.adjacencyList[i].push_back(this->adjacencyList[i][j]);

		for (uint j = 0; j < source.adjacencyList[i].size(); j++)
		{
			bool found = false;

			for (uint k = 0; k < sumGraph.adjacencyList[i].size(); k++)
				if (sumGraph.adjacencyList[i][k] == source.adjacencyList[i][j])
					found = true;

			if (!found)
				sumGraph.adjacencyList[i].push_back(source.adjacencyList[i][j]);
		}
	}

	return sumGraph;
}

DirectedGraph DirectedGraph::operator-(const DirectedGraph& source) const
{
	// TODO: better way to handle this
	if (this->vertices != this->vertices || source.vertices == 0)
		return DirectedGraph();

	DirectedGraph difGraph;
	difGraph.vertices = this->vertices;
	difGraph.adjacencyList.resize(difGraph.vertices);

	for (uint i = 0; i < this->adjacencyList.size(); i++)
	{
		for (uint j = 0; j < this->adjacencyList[i].size(); j++)
		{
			bool found = false;
			for (uint k = 0; k < this->adjacencyList[i].size(); k++)
				if (this->adjacencyList[i][j] == source.adjacencyList[i][k])
					found = true;

			if (!found)
				difGraph.adjacencyList[i].push_back(this->adjacencyList[i][j]);
		}
	}

	return difGraph;
}

bool DirectedGraph::operator==(const DirectedGraph& source) const
{
	if (this->vertices != source.vertices || this->edges != source.edges)
		return false;

	if (((*this) + source).edges != this->edges)
		return false;

	return true;
}

std::istream& operator>>(std::istream& is, DirectedGraph& graph)
{
	is >> graph.vertices >> graph.edges;
	graph.adjacencyList.resize(graph.vertices);

	for (uint i = 0; i < graph.edges; i++)
	{
		uint x, y;
		is >> x >> y;
		graph.adjacencyList[x].push_back(y);
	}

	return is;
}

std::ifstream& operator>>(std::ifstream& ifs, DirectedGraph& graph)
{
	ifs >> graph.vertices >> graph.edges;
	graph.adjacencyList.resize(graph.vertices);

	for (uint i = 0; i < graph.edges; i++)
	{
		uint x, y;
		ifs >> x >> y;
		graph.adjacencyList[x].push_back(y);
	}

	return ifs;
}

// -- End of operator overloading

// -- Private members

void DirectedGraph::topologicalSort(uint vertex, std::vector<bool>& visited, std::stack<uint>& topSort) const
{
	visited[vertex] = true;

	for (uint i = 0; i < adjacencyList[vertex].size(); i++)
		if (!visited[adjacencyList[vertex][i]])
			topologicalSort(adjacencyList[vertex][i], visited, topSort);

	topSort.push(vertex);
}

// -- End of private members
