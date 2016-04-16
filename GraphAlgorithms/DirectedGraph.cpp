#include "PCH.h"
#include "DirectedGraph.h"

DirectedGraph::DirectedGraph(std::ifstream& ifs, bool const& weighted)
{
	_weighted = weighted;
	ifs >> _vertices >> _edges;
	_adjacencyList.resize(_vertices);

	if (!isWeighted())
		for (uint32_t i = 0; i < _edges; i++)
		{
			uint32_t x, y;
			ifs >> x >> y;
			_adjacencyList[x].push_back(std::make_pair(y, 0));
		}
	else
		for (uint32_t i = 0; i < _edges; i++)
		{
			uint32_t x, y, w;
			ifs >> x >> y >> w;
			_adjacencyList[x].push_back(std::make_pair(y, w));
		}
}

uint32_t DirectedGraph::getDegree(uint32_t const& vertex) const
{
	if (!isValidVertex(vertex))
		return 0;

	return getInDegree(vertex) + getOutDegree(vertex);
}

uint32_t DirectedGraph::getInDegree(uint32_t const& vertex) const
{
	if (!isValidVertex(vertex))
		return 0;

	uint32_t count = 0;

	for (uint32_t i = 0; i < _adjacencyList.size(); i++)
	{
		for (uint32_t j = 0; j < _adjacencyList[i].size(); j++)
			if (_adjacencyList[i][j].first == vertex)
				count++;
	}

	return count;
}

uint32_t DirectedGraph::getOutDegree(uint32_t const& vertex) const
{
	if (!isValidVertex(vertex))
		return 0;

	return static_cast<uint32_t>(_adjacencyList[vertex].size());
}

uint32_t DirectedGraph::getMinDegree() const
{
	if (!_vertices || _vertices == 1 || !_edges)
		return 0;

	uint32_t minDegree = getDegree(0);

	for (unsigned int i = 1; i < _vertices; i++)
		if (getDegree(i) < minDegree)
			minDegree = getDegree(i);

	return minDegree;
}

uint32_t DirectedGraph::getMinInDegree() const
{
	if (!_vertices || _vertices == 1 || !_edges)
		return 0;

	uint32_t minInDegree = getInDegree(0);

	for (unsigned int i = 1; i < _vertices; i++)
		if (getInDegree(i) < minInDegree)
			minInDegree = getInDegree(i);

	return minInDegree;
}

uint32_t DirectedGraph::getMinOutDegree() const
{
	if (!_vertices || _vertices == 1 || !_edges)
		return 0;

	uint32_t minOutDegree = getOutDegree(0);

	for (unsigned int i = 1; i < _vertices; i++)
		if (getOutDegree(i) < minOutDegree)
			minOutDegree = getOutDegree(i);

	return minOutDegree;
}

uint32_t DirectedGraph::getMaxDegree() const
{
	if (!_vertices || _vertices == 1 || !_edges)
		return 0;

	uint32_t maxDegree = getDegree(0);

	for (uint32_t i = 1; i < _vertices; i++)
		if (getDegree(i) > maxDegree)
			maxDegree = getDegree(i);

	return maxDegree;
}

uint32_t DirectedGraph::getMaxInDegree() const
{
	if (!_vertices || _vertices == 1 || !_edges)
		return 0;

	uint32_t maxInDegree = getInDegree(0);

	for (uint32_t i = 1; i < _vertices; i++)
		if (getInDegree(i) > maxInDegree)
			maxInDegree = getInDegree(i);

	return maxInDegree;
}

uint32_t DirectedGraph::getMaxOutDegree() const
{
	if (!_vertices || _vertices == 1 || !_edges)
		return 0;

	uint32_t maxOutDegree = getOutDegree(0);

	for (uint32_t i = 1; i < _vertices; i++)
		if (getOutDegree(i) > maxOutDegree)
			maxOutDegree = getOutDegree(i);

	return maxOutDegree;
}

bool DirectedGraph::isComplete() const
{
	for (uint32_t i = 0; i < _vertices; i++)
	{
		if (_adjacencyList[i].size() < _vertices - 1)
			return false;

		Vector<bool> visited(_vertices, false);
		visited[i] = true;

		for (uint32_t j = 0; j < _adjacencyList[i].size(); j++)
			visited[_adjacencyList[i][j].first] = true;

		for (uint32_t j = 0; j < visited.size(); j++)
			if (!visited[j])
				return false;
	}

	return true;
}

bool DirectedGraph::isRegular() const
{
	for (uint32_t i = 1; i < _vertices; i++)
		if (getInDegree(i) != getInDegree(i - 1) || getOutDegree(i) != getOutDegree(i - 1))
			return false;

	return true;
}

bool DirectedGraph::isStronglyConnected() const
{
	for (uint32_t i = 0; i < _vertices; i++)
	{
		Vector<int> roadDistance = getRoadDistance(i);

		for (uint32_t j = 0; j < roadDistance.size(); j++)
			if (roadDistance[j] == -1)
				return false;
	}

	return true;
}

Stack<uint32_t> DirectedGraph::getTopologicalSort() const
{
	Stack<uint32_t> topSort;
	Vector<bool> visited(_vertices, false);

	for (uint32_t vertex = 0; vertex < _vertices; vertex++)
		if (!visited[vertex])
			topologicalSort(vertex, visited, topSort);

	return topSort;
}

Vector<Vector<bool>> DirectedGraph::getRoadMatrix() const
{
	Vector<Vector<bool>> roadMatrix(_vertices);

	for (uint32_t i = 0; i < _vertices; i++)
	{
		Vector<int> roadDistance = getRoadDistance(i);

		for (uint32_t j = 0; j < roadDistance.size(); j++)
			if (roadDistance[j] != -1)
				roadMatrix[i][j] = true;
	}

	return roadMatrix;
}

DirectedGraph& DirectedGraph::operator=(DirectedGraph const& source)
{
	if (this == &source)
		return *this;

	_weighted = source._weighted;
	_vertices = source._vertices;
	_edges = source._edges;
	_adjacencyList = source._adjacencyList;

	return *this;
}

DirectedGraph DirectedGraph::operator+(DirectedGraph const& source) const
{
	if (this->_vertices != source._vertices || this->_vertices == 0)
		return DirectedGraph();

	DirectedGraph sumGraph;
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

DirectedGraph DirectedGraph::operator-(DirectedGraph const& source) const
{
	// TODO: better way to handle this
	if (this->_vertices != this->_vertices || source._vertices == 0)
		return DirectedGraph();

	DirectedGraph difGraph;
	difGraph._vertices = this->_vertices;
	difGraph._adjacencyList.resize(difGraph._vertices);

	for (uint32_t i = 0; i < this->_adjacencyList.size(); i++)
	{
		for (uint32_t j = 0; j < this->_adjacencyList[i].size(); j++)
		{
			bool found = false;
			for (uint32_t k = 0; k < this->_adjacencyList[i].size(); k++)
				if (this->_adjacencyList[i][j] == source._adjacencyList[i][k])
					found = true;

			if (!found)
				difGraph._adjacencyList[i].push_back(this->_adjacencyList[i][j]);
		}
	}

	return difGraph;
}

bool DirectedGraph::operator==(DirectedGraph const& source) const
{
	if (this->_vertices != source._vertices || this->_edges != source._edges)
		return false;

	if (((*this) + source)._edges != this->_edges)
		return false;

	return true;
}

void DirectedGraph::topologicalSort(uint32_t const& vertex, Vector<bool>& visited, Stack<uint32_t>& topSort) const
{
	visited[vertex] = true;

	for (uint32_t i = 0; i < _adjacencyList[vertex].size(); i++)
		if (!visited[_adjacencyList[vertex][i].first])
			topologicalSort(_adjacencyList[vertex][i].first, visited, topSort);

	topSort.push(vertex);
}

std::istream& operator>>(std::istream& is, DirectedGraph& graph)
{
	uint16_t weighted;
	is >> graph._vertices >> graph._edges >> weighted;

	graph._adjacencyList.~vector();
	graph._adjacencyList.resize(graph._vertices);

	graph._weighted = weighted ? true : false;

	if (!graph.isWeighted())
		for (uint32_t i = 0; i < graph._edges; i++)
		{
			uint32_t x, y;
			is >> x >> y;
			graph._adjacencyList[x].push_back(std::make_pair(y, 0));
		}
	else
		for (uint32_t i = 0; i < graph._edges; i++)
		{
			uint32_t x, y, w;
			is >> x >> y >> w;
			graph._adjacencyList[x].push_back(std::make_pair(y, w));
		}

	return is;
}

std::ifstream& operator>>(std::ifstream& ifs, DirectedGraph& graph)
{
	uint16_t weighted;
	ifs >> graph._vertices >> graph._edges >> weighted;

	graph._adjacencyList.~vector();
	graph._adjacencyList.resize(graph._vertices);

	graph._weighted = weighted ? true : false;

	if (!graph.isWeighted())
		for (uint32_t i = 0; i < graph._edges; i++)
		{
			uint32_t x, y;
			ifs >> x >> y;
			graph._adjacencyList[x].push_back(std::make_pair(y, 0));
		}
	else
		for (uint32_t i = 0; i < graph._edges; i++)
		{
			uint32_t x, y, w;
			ifs >> x >> y >> w;
			graph._adjacencyList[x].push_back(std::make_pair(y, w));
		}

	return ifs;
}

