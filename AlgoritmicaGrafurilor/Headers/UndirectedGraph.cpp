#include "UndirectedGraph.h"

// -- Public methods

UndirectedGraph::UndirectedGraph(std::ifstream &input)
{
	input >> vertices >> edges;
	graph.resize(vertices);

	for (uint i = 0; i < edges; i++)
	{
		uint x, y;
		input >> x >> y;
		graph[x].push_back(y);
		graph[y].push_back(x);
	}
}

UndirectedGraph::~UndirectedGraph()
{
	vertices = edges = 0;
	graph.~vector();
}

uint UndirectedGraph::getDegree(uint vertex) const
{
	if (!isValidVertex(vertex))
		return 0;

	return graph[vertex].size();
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
			if (!visited[graph[element][i]])
			{
				queue.push(graph[element][i]);
				visited[graph[element][i]] = true;
				color[graph[element][i]] = (color[element] == 0 ? 1 : 0);
			}
			else if (color[element] == color[graph[element][i]])
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

std::vector<uint> UndirectedGraph::getRoadDistance(uint vertex) const
{
	if (!isValidVertex(vertex))
		return std::vector<uint>();

	std::vector<bool> visited(vertices);
	std::queue<uint> queue;
	std::vector<uint> roadDistance(vertices, 0x3f3f3f3f);

	queue.push(vertex);
	visited[vertex] = true;
	roadDistance[vertex] = 0;

	while (!queue.empty())
	{
		uint element = queue.front();

		for (uint i = 0; i < getDegree(element); i++)
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

// -- Private methods

bool UndirectedGraph::isValidVertex(uint vertex) const
{
	if (vertex < 0 || vertex > vertices - 1)
		return false;

	return true;
}

// -- Overloaded Operators

UndirectedGraph& UndirectedGraph::operator=(const UndirectedGraph& source)
{
	if (this == &source)
		return *this;

	vertices = source.vertices;
	edges = source.edges;
	graph = source.graph;

	return *this;
}

std::ofstream& operator<<(std::ofstream& ofs, const UndirectedGraph& g)
{
	for (uint i = 0; i < g.graph.size(); i++)
	{
		ofs << i << " | ";
		for (uint j = 0; j < g.graph[i].size(); j++)
			ofs << g.graph[i][j] << " ";
		ofs << "\n";
	}

	return ofs;
}

std::ostream& operator<<(std::ostream& os, const UndirectedGraph& g)
{
	for (uint i = 0; i < g.graph.size(); i++)
	{
		os << i << " | ";
		for (uint j = 0; j < g.graph[i].size(); j++)
			os << g.graph[i][j] << " ";
		os << "\n";
	}

	return os;
}

std::ifstream& operator>>(std::ifstream& ifs, UndirectedGraph& g)
{
	ifs >> g.vertices >> g.edges;
	g.graph.resize(g.vertices);

	for (uint i = 0; i < g.edges; i++)
	{
		uint x, y;
		ifs >> x >> y;
		g.graph[x].push_back(y);
		g.graph[y].push_back(x);
	}

	return ifs;
}

std::istream& operator>>(std::istream& is, UndirectedGraph& g)
{
	is >> g.vertices >> g.edges;
	g.graph.resize(g.vertices);

	for (uint i = 0; i < g.edges; i++)
	{
		uint x, y;
		is >> x >> y;
		g.graph[x].push_back(y);
		g.graph[y].push_back(x);
	}

	return is;
}

UndirectedGraph operator+(const UndirectedGraph& graphOne, const UndirectedGraph& graphTwo)
{
	// TODO: better way to handle this
	if (graphOne.vertices != graphTwo.vertices || graphOne.vertices == 0)
		return UndirectedGraph();

	UndirectedGraph sumGraph;
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

UndirectedGraph operator-(const UndirectedGraph& graphOne, const UndirectedGraph& graphTwo)
{
	// TODO: better way to handle this
	if (graphOne.vertices != graphTwo.vertices || graphOne.vertices == 0)
		return UndirectedGraph();

	UndirectedGraph difGraph;
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

bool operator==(const UndirectedGraph& graphOne, const UndirectedGraph& graphTwo)
{
	if (graphOne.vertices != graphTwo.vertices || graphOne.edges != graphTwo.edges || graphOne.graph != graphTwo.graph)
		return false;

	if ((graphOne + graphTwo).edges != graphOne.edges)
		return false;

	return true;
}
