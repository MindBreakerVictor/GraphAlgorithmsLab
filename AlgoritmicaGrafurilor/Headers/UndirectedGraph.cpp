#include "UndirectedGraph.h"

// -- Public methods

UndirectedGraph::UndirectedGraph(std::ifstream& ifs)
{
	ifs >> vertices >> edges;
	adjacencyList.resize(vertices);

	for (uint i = 0; i < edges; i++)
	{
		uint x, y;
		ifs >> x >> y;
		adjacencyList[x].push_back(y);
		adjacencyList[y].push_back(x);
	}
}

uint UndirectedGraph::getDegree(uint vertex) const
{
	if (!isValidVertex(vertex))
		return 0;

	return adjacencyList[vertex].size();
}

uint UndirectedGraph::getMinDegree() const
{
	if (!vertices || vertices == 1 || !edges)
		return 0;

	uint minDegree = getDegree(0);

	for (unsigned int i = 1; i < vertices; i++)
		if (getDegree(i) < minDegree)
			minDegree = getDegree(i);

	return minDegree;
}

uint UndirectedGraph::getMaxDegree() const
{
	if (!vertices || vertices == 1 || !edges)
		return 0;

	uint maxDegree = getDegree(0);

	for (uint i = 1; i < vertices; i++)
		if (getDegree(i) > maxDegree)
			maxDegree = getDegree(i);

	return maxDegree;
}

bool UndirectedGraph::isComplete() const
{
	if ((vertices * (vertices - 1)) / 2 == edges)
		return true;

	return false;
}

bool UndirectedGraph::isRegular() const
{
	for (uint i = 1; i < vertices; i++)
		if (getDegree(i) != getDegree(i - 1))
			return false;

	return true;
}

bool UndirectedGraph::isConnected() const
{
	if (!vertices)
		return false;

	if (vertices == 1)
		return true;

	if (getConnectedComponents().size() == 1)
		return true;

	return false;
}

bool UndirectedGraph::isHamiltonian() const
{
	if (vertices < 3)
		return false;

	if (isComplete())
		return true;

	for (uint i = 0; i < vertices; i++)
		if (getDegree(i) < vertices / 2)
			return false;

	return true;
}

bool UndirectedGraph::isEulerian() const
{
	if (!isConnected())
		return false;

	for (uint i = 0; i < vertices; i++)
		if (getDegree(i) % 2 != 0)
			return false;

	return true;
}

bool UndirectedGraph::isBipartite() const
{
	if (!vertices || !edges)
		return false;

	std::vector<bool> visited(vertices);
	std::vector<char> color(vertices);
	std::queue<uint> queue;

	queue.push(0);
	visited[0] = true;
	color[0] = 0;

	while (!queue.empty())
	{
		uint element = queue.front();

		for (uint i = 0; i < getDegree(element); i++)
			if (!visited[adjacencyList[element][i]])
			{
				queue.push(adjacencyList[element][i]);
				visited[adjacencyList[element][i]] = true;
				color[adjacencyList[element][i]] = (color[element] == 0 ? 1 : 0);
			}
			else if (color[element] == color[adjacencyList[element][i]])
				return false;

		queue.pop();
	}

	return true;
}

std::vector<uint> UndirectedGraph::breadthFirstSearch(uint vertex) const
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

		for (uint i = 0; i < getDegree(element); i++)
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

std::vector<uint> UndirectedGraph::depthFirstSearch(uint vertex) const
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

		for (index = 0; index < getDegree(element) && !found; index++)
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

std::vector<std::vector<uint>> UndirectedGraph::getConnectedComponents() const
{
	std::vector<std::vector<uint>> connectedComponents;
	std::vector<bool> visited(vertices, false);

	for (uint i = 0; i < vertices; i++)
	{
		if (!visited[i])
		{
			connectedComponents.push_back(depthFirstSearch(i));

			for (uint j = 0; j < connectedComponents.back().size(); j++)
				visited[connectedComponents.back()[j]] = true;
		}
	}

	return connectedComponents;
}

std::vector<std::vector<bool>> UndirectedGraph::getRoadMatrix() const
{
	std::vector<std::vector<bool>> roadMatrix(vertices);
	std::vector<std::vector<uint>> connectedComponents = getConnectedComponents();
	
	for (uint i = 0; i < roadMatrix.size(); i++)
		roadMatrix[i].resize(vertices, false);

	for (uint i = 0; i < connectedComponents.size(); i++)
		for (uint j = 0; j < connectedComponents[i].size() - 1; j++)
			for (uint k = j + 1; k < connectedComponents[i].size(); k++)
			{
				roadMatrix[connectedComponents[i][j]][connectedComponents[i][k]] = true;
				roadMatrix[connectedComponents[i][k]][connectedComponents[i][j]] = true;
			}

	return roadMatrix;
}

std::vector<int> UndirectedGraph::getRoadDistance(uint vertex) const
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

		for (uint i = 0; i < getDegree(element); i++)
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

// -- Overloaded Operators

UndirectedGraph& UndirectedGraph::operator=(const UndirectedGraph& source)
{
	if (this == &source)
		return *this;

	vertices = source.vertices;
	edges = source.edges;
	adjacencyList = source.adjacencyList;

	return *this;
}

UndirectedGraph UndirectedGraph::operator+(const UndirectedGraph& source)
{
	// TODO: better way to handle this
	if (this->vertices != source.vertices || this->vertices == 0)
		return UndirectedGraph();

	UndirectedGraph sumGraph;
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

UndirectedGraph UndirectedGraph::operator-(const UndirectedGraph& source)
{
	// TODO: better way to handle this
	if (this->vertices != source.vertices || this->vertices == 0)
		return UndirectedGraph();

	UndirectedGraph difGraph;
	difGraph.vertices = this->vertices;
	difGraph.adjacencyList.resize(difGraph.vertices);

	for (uint i = 0; i < this->adjacencyList.size(); i++)
	{
		for (uint j = 0; j < this->adjacencyList[i].size(); j++)
		{
			bool found = false;
			for (uint k = 0; k < source.adjacencyList[i].size(); k++)
				if (this->adjacencyList[i][j] == source.adjacencyList[i][k])
					found = true;

			if (!found)
				difGraph.adjacencyList[i].push_back(this->adjacencyList[i][j]);
		}
	}

	return difGraph;
}

bool UndirectedGraph::operator==(const UndirectedGraph& source)
{
	if (this->vertices != source.vertices || this->edges != source.edges || this->adjacencyList != source.adjacencyList)
		return false;

	if (((*this) + source).edges != this->edges)
		return false;

	return true;
}

std::istream& operator>>(std::istream& is, UndirectedGraph& graph)
{
	is >> graph.vertices >> graph.edges;
	graph.adjacencyList.resize(graph.vertices);

	for (uint i = 0; i < graph.edges; i++)
	{
		uint x, y;
		is >> x >> y;
		graph.adjacencyList[x].push_back(y);
		graph.adjacencyList[y].push_back(x);
	}

	return is;
}

std::ifstream& operator>>(std::ifstream& ifs, UndirectedGraph& graph)
{
	ifs >> graph.vertices >> graph.edges;
	graph.adjacencyList.resize(graph.vertices);

	for (uint i = 0; i < graph.edges; i++)
	{
		uint x, y;
		ifs >> x >> y;
		graph.adjacencyList[x].push_back(y);
		graph.adjacencyList[y].push_back(x);
	}

	return ifs;
}
