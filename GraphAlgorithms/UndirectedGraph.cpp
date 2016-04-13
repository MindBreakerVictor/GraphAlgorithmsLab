#include "PCH.h"
#include "UndirectedGraph.h"

UndirectedGraph::UndirectedGraph(std::ifstream& ifs)
{
	ifs >> _vertices >> _edges;
	_adjacencyList.resize(_vertices);

	for (uint32_t i = 0; i < _edges; i++)
	{
		uint32_t x, y;
		ifs >> x >> y;
		_adjacencyList[x].push_back(y);
		_adjacencyList[y].push_back(x);
	}
}

uint32_t UndirectedGraph::getDegree(uint32_t vertex) const
{
	if (!isValidVertex(vertex))
		return 0;

	return (uint32_t)_adjacencyList[vertex].size();
}

uint32_t UndirectedGraph::getMinDegree() const
{
	if (!_vertices || _vertices == 1 || !_edges)
		return 0;

	uint32_t minDegree = getDegree(0);

	for (unsigned int i = 1; i < _vertices; i++)
		if (getDegree(i) < minDegree)
			minDegree = getDegree(i);

	return minDegree;
}

uint32_t UndirectedGraph::getMaxDegree() const
{
	if (!_vertices || _vertices == 1 || !_edges)
		return 0;

	uint32_t maxDegree = getDegree(0);

	for (uint32_t i = 1; i < _vertices; i++)
		if (getDegree(i) > maxDegree)
			maxDegree = getDegree(i);

	return maxDegree;
}

bool UndirectedGraph::isComplete() const
{
	if ((_vertices * (_vertices - 1)) / 2 == _edges)
		return true;

	return false;
}

bool UndirectedGraph::isRegular() const
{
	for (uint32_t i = 1; i < _vertices; i++)
		if (getDegree(i) != getDegree(i - 1))
			return false;

	return true;
}

bool UndirectedGraph::isConnected() const
{
	if (!_vertices)
		return false;

	if (_vertices == 1)
		return true;

	if (!_edges)
		return false;

	if (getConnectedComponents().size() == 1)
		return true;

	return false;
}

bool UndirectedGraph::isHamiltonian() const
{
	if (_vertices < 3)
		return false;

	if (isComplete())
		return true;

	for (uint32_t i = 0; i < _vertices; i++)
		if (getDegree(i) < _vertices / 2)
			return false;

	return true;
}

bool UndirectedGraph::isEulerian() const
{
	if (!isConnected())
		return false;

	for (uint32_t i = 0; i < _vertices; i++)
		if (getDegree(i) % 2 != 0)
			return false;

	return true;
}

bool UndirectedGraph::isBipartite() const
{
	if (!_vertices || !_edges)
		return false;

	Vector<bool> visited(_vertices);
	Vector<char> color(_vertices);
	Queue<uint32_t> queue;

	queue.push(0);
	visited[0] = true;
	color[0] = 0;

	while (!queue.empty())
	{
		uint32_t element = queue.front();

		for (uint32_t i = 0; i < getDegree(element); i++)
			if (!visited[_adjacencyList[element][i]])
			{
				queue.push(_adjacencyList[element][i]);
				visited[_adjacencyList[element][i]] = true;
				color[_adjacencyList[element][i]] = (color[element] == 0 ? 1 : 0);
			}
			else if (color[element] == color[_adjacencyList[element][i]])
				return false;

		queue.pop();
	}

	return true;
}

bool UndirectedGraph::isBiconnected() const
{
	if (!_vertices || !_edges)
		return false;

	Vector<bool> visited(_vertices, false);
	Vector<int> parent(_vertices, -1);
	Vector<uint32_t> discoveryTime(_vertices);
	Vector<uint32_t> low(_vertices);

	// Check if the graph has at least one articulation point
	if (hasArticulationPoint(0, visited, parent, discoveryTime, low))
		return false;

	// Check if the graph is connected
	for (uint32_t i = 0; i < visited.size(); i++)
		if (!visited[i])
			return false;

	return true;
}

Vector<Vector<bool>> UndirectedGraph::getRoadMatrix() const
{
	Vector<Vector<bool>> roadMatrix(_vertices);
	Vector<Vector<uint32_t>> connectedComponents = getConnectedComponents();

	for (uint32_t i = 0; i < roadMatrix.size(); i++)
		roadMatrix[i].resize(_vertices, false);

	for (uint32_t i = 0; i < connectedComponents.size(); i++)
		for (uint32_t j = 0; j < connectedComponents[i].size() - 1; j++)
			for (uint32_t k = j + 1; k < connectedComponents[i].size(); k++)
			{
				roadMatrix[connectedComponents[i][j]][connectedComponents[i][k]] = true;
				roadMatrix[connectedComponents[i][k]][connectedComponents[i][j]] = true;
			}

	return roadMatrix;
}

Vector<Vector<uint32_t>> UndirectedGraph::getConnectedComponents() const
{
	Vector<Vector<uint32_t>> connectedComponents;
	Vector<bool> visited(_vertices, false);

	for (uint32_t i = 0; i < _vertices; i++)
	{
		if (!visited[i])
		{
			connectedComponents.push_back(depthFirstSearch(i));

			for (uint32_t j = 0; j < connectedComponents.back().size(); j++)
				visited[connectedComponents.back()[j]] = true;
		}
	}

	return connectedComponents;
}

Vector<Vector<uint32_t>> UndirectedGraph::getBiconnectedComponents() const
{
	Stack<uint32_t> stack;
	Vector<int> parent(_vertices, -1);						// Represents the parent of the vertex in DFS-tree.
	Vector<uint32_t> low(_vertices, 0);						// Represents the lowest depth of a vertex connected to the index vertex through a back-edge in DFS-tree.
	Vector<uint32_t> depth(_vertices, 0);					// Represents the depth of the vertex in DFS-tree.
	Vector<Vector<uint32_t>> biconnectedComponents;

	for (uint32_t vertex = 0; vertex < _vertices; vertex++)
		if (!depth[vertex])
			getBiconnectedComponents(vertex, parent, depth, low, stack, biconnectedComponents);

	return biconnectedComponents;
}

Vector<uint32_t> UndirectedGraph::getArticulationPoints() const
{
	Vector<bool> visited(_vertices, false);
	Vector<int> parent(_vertices, -1);
	Vector<uint32_t> discoveryTime(_vertices);
	Vector<uint32_t> low(_vertices);
	Vector<uint32_t> articulationPoints;

	for (uint32_t i = 0; i < _vertices; i++)
		if (!visited[i])
			articulationPoint(i, visited, parent, discoveryTime, low, articulationPoints);

	return articulationPoints;
}

void UndirectedGraph::articulationPoint(uint32_t vertex, Vector<bool>& visited, Vector<int>& parent,
	Vector<uint32_t>& discoveryTime, Vector<uint32_t>& low, Vector<uint32_t>& articulationPoints) const
{
	if (!isValidVertex(vertex))
		return;

	static uint32_t time = 0;
	uint32_t children = 0;
	visited[vertex] = true;
	discoveryTime[vertex] = low[vertex] = ++time;

	for (uint32_t i = 0; i < _adjacencyList[vertex].size(); i++)
	{
		if (!visited[_adjacencyList[vertex][i]])
		{
			children++;
			parent[_adjacencyList[vertex][i]] = vertex;
			articulationPoint(_adjacencyList[vertex][i], visited, parent, discoveryTime, low, articulationPoints);

			if (low[_adjacencyList[vertex][i]] < low[vertex])
				low[vertex] = low[_adjacencyList[vertex][i]];

			if ((parent[vertex] == -1 && children > 1) || (parent[vertex] != -1 && low[_adjacencyList[vertex][i]] >= discoveryTime[vertex]))
				articulationPoints.push_back(vertex);
		}
		else if ((_adjacencyList[vertex][i] != parent[vertex]) && (discoveryTime[_adjacencyList[vertex][i]] < low[vertex]))
			low[vertex] = discoveryTime[_adjacencyList[vertex][i]];
	}
}

// Same as articulationPoint, just we don't need all articulation points to check if a graph is biconnected.
bool UndirectedGraph::hasArticulationPoint(uint32_t vertex, Vector<bool>& visited, Vector<int>& parent,
	Vector<uint32_t>& discoveryTime, Vector<uint32_t>& low) const
{
	if (!isValidVertex(vertex))
		return false;

	static uint32_t time = 0;
	uint32_t children = 0;
	visited[vertex] = true;
	discoveryTime[vertex] = low[vertex] = ++time;

	for (uint32_t i = 0; i < _adjacencyList[vertex].size(); i++)
	{
		if (!visited[_adjacencyList[vertex][i]])
		{
			children++;
			parent[_adjacencyList[vertex][i]] = vertex;
			
			if (hasArticulationPoint(_adjacencyList[vertex][i], visited, parent, discoveryTime, low))
				return true;

			if (low[_adjacencyList[vertex][i]] < low[vertex])
				low[vertex] = low[_adjacencyList[vertex][i]];

			if ((parent[vertex] == -1 && children > 1) || 
				(parent[vertex] != -1 && low[_adjacencyList[vertex][i]] >= discoveryTime[vertex]))
				return true;
		}
		else if ((_adjacencyList[vertex][i] != parent[vertex]) && 
			(discoveryTime[_adjacencyList[vertex][i]] < low[vertex]))
			low[vertex] = discoveryTime[_adjacencyList[vertex][i]];
	}

	return false;
}

void UndirectedGraph::getBiconnectedComponents(uint32_t vertex, Vector<int>& parent, Vector<uint32_t>& depth, 
	Vector<uint32_t>& low, Stack<uint32_t>& stack, Vector<Vector<uint32_t>>& biconnectedComponents) const
{
	if (!isValidVertex(vertex))
		return;

	static uint32_t currentDepth = 0;
	stack.push(vertex);
	depth[vertex] = low[vertex] = ++currentDepth;

	for (Vector<uint32_t>::const_iterator neighbour = _adjacencyList[vertex].begin(); neighbour != _adjacencyList[vertex].end(); neighbour++)
	{
		if (!depth[*neighbour])
		{
			parent[*neighbour] = vertex;
			getBiconnectedComponents(*neighbour, parent, depth, low, stack, biconnectedComponents);
			low[vertex] = std::min(low[vertex], low[*neighbour]);

			// Check if vertex is an articulation point.
			if (low[*neighbour] >= depth[vertex])
			{
				biconnectedComponents.push_back(Vector<uint32_t>());
				Vector<Vector<uint32_t>>::reverse_iterator iterator = biconnectedComponents.rbegin();
				while (stack.top() != *neighbour)
				{
					iterator->push_back(stack.top());
					stack.pop();
				}
				iterator->push_back(*neighbour);
				stack.pop();
				iterator->push_back(vertex);
			}
		}
		// Check if neighbour is an ancestor of vertex in dfs tree.
		else if (*neighbour != parent[vertex])
			low[vertex] = std::min(low[vertex], depth[*neighbour]);
	}
}

UndirectedGraph& UndirectedGraph::operator=(const UndirectedGraph& source)
{
	if (this == &source)
		return *this;

	_vertices = source._vertices;
	_edges = source._edges;
	_adjacencyList = source._adjacencyList;

	return *this;
}

UndirectedGraph UndirectedGraph::operator+(const UndirectedGraph& source)
{
	// TODO: better way to handle this
	if (this->_vertices != source._vertices || this->_vertices == 0)
		return UndirectedGraph();

	UndirectedGraph sumGraph;
	sumGraph._vertices = this->_vertices;
	sumGraph._adjacencyList.resize(sumGraph._vertices);

	for (uint32_t i = 0; i < sumGraph._vertices; i++)
	{
		for (uint32_t j = 0; j < this->_adjacencyList[i].size(); j++)
			sumGraph._adjacencyList[i].push_back(this->_adjacencyList[i][j]);

		for (uint32_t j = 0; j < source._adjacencyList[i].size(); j++)
		{
			bool found = false;

			for (uint32_t k = 0; k < sumGraph._adjacencyList[i].size(); k++)
				if (sumGraph._adjacencyList[i][k] == source._adjacencyList[i][j])
					found = true;

			if (!found)
				sumGraph._adjacencyList[i].push_back(source._adjacencyList[i][j]);
		}
	}

	return sumGraph;
}

UndirectedGraph UndirectedGraph::operator-(const UndirectedGraph& source)
{
	// TODO: better way to handle this
	if (this->_vertices != source._vertices || this->_vertices == 0)
		return UndirectedGraph();

	UndirectedGraph difGraph;
	difGraph._vertices = this->_vertices;
	difGraph._adjacencyList.resize(difGraph._vertices);

	for (uint32_t i = 0; i < this->_adjacencyList.size(); i++)
	{
		for (uint32_t j = 0; j < this->_adjacencyList[i].size(); j++)
		{
			bool found = false;
			for (uint32_t k = 0; k < source._adjacencyList[i].size(); k++)
				if (this->_adjacencyList[i][j] == source._adjacencyList[i][k])
					found = true;

			if (!found)
				difGraph._adjacencyList[i].push_back(this->_adjacencyList[i][j]);
		}
	}

	return difGraph;
}

bool UndirectedGraph::operator==(const UndirectedGraph& source)
{
	if (this->_vertices != source._vertices || this->_edges != source._edges || this->_adjacencyList != source._adjacencyList)
		return false;

	if (((*this) + source)._edges != this->_edges)
		return false;

	return true;
}

std::istream& operator>>(std::istream& is, UndirectedGraph& graph)
{
	is >> graph._vertices >> graph._edges;
	graph._adjacencyList.resize(graph._vertices);

	for (uint32_t i = 0; i < graph._edges; i++)
	{
		uint32_t x, y;
		is >> x >> y;
		graph._adjacencyList[x].push_back(y);
		graph._adjacencyList[y].push_back(x);
	}

	return is;
}

std::ifstream& operator>>(std::ifstream& ifs, UndirectedGraph& graph)
{
	ifs >> graph._vertices >> graph._edges;
	graph._adjacencyList.resize(graph._vertices);

	for (uint32_t i = 0; i < graph._edges; i++)
	{
		uint32_t x, y;
		ifs >> x >> y;
		graph._adjacencyList[x].push_back(y);
		graph._adjacencyList[y].push_back(x);
	}

	return ifs;
}

