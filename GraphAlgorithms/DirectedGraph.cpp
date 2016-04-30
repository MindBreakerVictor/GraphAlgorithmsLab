#include "PCH.h"
#include "DirectedGraph.h"

DirectedGraph::DirectedGraph(std::ifstream& ifs, bool weighted)
{
	uint32_t vertices;
	_weighted = weighted;
	ifs >> vertices >> _edges;
	_adjacencyList.resize(vertices);

	if (!IsWeighted())
		for (uint32_t i = 0; i < GetEdges(); ++i)
		{
			uint32_t x, y;

			ifs >> x >> y;
			_adjacencyList[x].push_back(std::make_pair(y, 0));
		}
	else
		for (uint32_t i = 0; i < GetEdges(); ++i)
		{
			uint32_t x, y, w;

			ifs >> x >> y >> w;
			_adjacencyList[x].push_back(std::make_pair(y, w));
		}
}

bool DirectedGraph::IsComplete() const
{
	for (uint32_t i = 0; i < GetVertices(); ++i)
	{
		if (GetOutDegree(i) < (GetVertices() - 1))
			return false;

		Vector<bool> visited(GetVertices(), false);
		visited[i] = true;

		for (uint32_t j = 0; j < GetOutDegree(i); ++j)
			visited[_adjacencyList[i][j].first] = true;

		for (uint32_t j = 0; j < visited.size(); ++j)
			if (!visited[j])
				return false;
	}

	return true;
}

bool DirectedGraph::IsRegular() const
{
	for (uint32_t i = 1; i < GetVertices(); ++i)
		if ((GetInDegree(i) != GetInDegree(i - 1)) || (GetOutDegree(i) != GetOutDegree(i - 1)))
			return false;

	return true;
}

bool DirectedGraph::IsStronglyConnected() const
{
	for (uint32_t i = 0; i < GetVertices(); ++i)
	{
		Vector<int> roadDistance = GetRoadDistance(i);

		for (uint32_t j = 0; j < roadDistance.size(); ++j)
			if (roadDistance[j] == -1)
				return false;
	}

	return true;
}

Stack<uint32_t> DirectedGraph::GetTopologicalSort() const
{
	Stack<uint32_t> topSort;
	Vector<bool> visited(GetVertices(), false);

	for (uint32_t i = 0; i < GetVertices(); ++i)
		if (!visited[i])
			TopologicalSort(i, &visited, &topSort);

	return topSort;
}

Matrix<uint32_t> DirectedGraph::GetStronglyConnectedComponents() const
{
	Stack<uint32_t> stack;
	Vector<uint32_t> low(GetVertices(), 0);						// Represents the lowest depth of a vertex connected to the index vertex through a back-edge in DFS-tree.
	Vector<uint32_t> depth(GetVertices(), 0);					// Represents the depth of the vertex in DFS-tree.
	Vector<bool> isInStack(GetVertices(), false);
	Matrix<uint32_t> stronglyConnectedComponents;

	for (uint32_t i = 0; i < GetVertices(); ++i)
		if (!depth[i])
			GetStronglyConnectedComponents(i, &depth, &low, &isInStack, &stack, &stronglyConnectedComponents);

	return stronglyConnectedComponents;
}

Matrix<bool> DirectedGraph::GetRoadMatrix() const
{
	Matrix<bool> roadMatrix(GetVertices());

	for (uint32_t i = 0; i < GetVertices(); ++i)
	{
		Vector<int> roadDistance = GetRoadDistance(i);

		for (uint32_t j = 0; j < roadDistance.size(); ++j)
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
	_edges = source._edges;
	_adjacencyList = source._adjacencyList;

	return *this;
}

DirectedGraph DirectedGraph::operator+(DirectedGraph const& source) const
{
	if (this->GetVertices() != source.GetVertices() || this->GetVertices() == 0)
		return DirectedGraph();

	DirectedGraph sumGraph;
	sumGraph._adjacencyList.resize(this->GetVertices());

	for (uint32_t i = 0; i < sumGraph.GetVertices(); ++i)
	{
		for (uint32_t j = 0; j < this->GetOutDegree(i); ++j)
			sumGraph._adjacencyList[i].push_back(this->_adjacencyList[i][j]);

		for (uint32_t j = 0; j < source.GetOutDegree(i); ++j)
		{
			bool found = false;

			for (uint32_t k = 0; k < sumGraph.GetOutDegree(i); ++k)
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
	if (this->GetVertices() != this->GetVertices() || source.GetVertices() == 0)
		return DirectedGraph();

	DirectedGraph difGraph;
	difGraph._adjacencyList.resize(this->GetVertices());

	for (uint32_t i = 0; i < this->GetVertices(); ++i)
	{
		for (uint32_t j = 0; j < this->GetOutDegree(i); ++j)
		{
			bool found = false;
			for (uint32_t k = 0; k < this->GetOutDegree(i); ++k)
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
	if (this->GetVertices() != source.GetVertices() || this->GetEdges() != source.GetEdges())
		return false;

	if (((*this) + source).GetEdges() != this->GetEdges())
		return false;

	return true;
}

void DirectedGraph::TopologicalSort(uint32_t const& vertex, Vector<bool>* visited, Stack<uint32_t>* topSort) const
{
	(*visited)[vertex] = true;

	for (uint32_t i = 0; i < GetOutDegree(vertex); ++i)
		if (!(*visited)[_adjacencyList[vertex][i].first])
			TopologicalSort(_adjacencyList[vertex][i].first, visited, topSort);

	topSort->push(vertex);
}

void DirectedGraph::GetStronglyConnectedComponents(uint32_t const& vertex, Vector<uint32_t>* depth, Vector<uint32_t>* low,
	Vector<bool>* isInStack, Stack<uint32_t>* stack, Matrix<uint32_t>* stronglyConnectedComponents) const
{
	if (!IsValidVertex(vertex))
		return;

	static uint32_t currentDepth = 0;
	stack->push(vertex);
	(*isInStack)[vertex] = true;
	(*depth)[vertex] = (*low)[vertex] = ++currentDepth;

	for (AdjacencyListConstIterator neighbour = _adjacencyList[vertex].begin(); neighbour != _adjacencyList[vertex].end(); ++neighbour)
	{
		if (!(*depth)[neighbour->first])
		{
			GetStronglyConnectedComponents(neighbour->first, depth, low, isInStack, stack, stronglyConnectedComponents);
			(*low)[vertex] = std::min((*low)[vertex], (*low)[neighbour->first]);
		}
		else if ((*isInStack)[neighbour->first])
			(*low)[vertex] = std::min((*low)[vertex], (*depth)[neighbour->first]);
	}

	if ((*low)[vertex] == (*depth)[vertex])
	{
		Matrix<uint32_t>::reverse_iterator itr;
		stronglyConnectedComponents->push_back(Vector<uint32_t>());
		itr = stronglyConnectedComponents->rbegin();

		while (stack->top() != vertex)
		{
			itr->push_back(stack->top());
			(*isInStack)[stack->top()] = false;
			stack->pop();
		}

		itr->push_back(stack->top());
		(*isInStack)[stack->top()] = false;
		stack->pop();
	}
}

std::istream& operator>>(std::istream& is, DirectedGraph& graph)
{
	uint16_t weighted;
	uint32_t vertices;
	is >> vertices >> graph._edges >> weighted;

	graph._adjacencyList.~vector();
	graph._adjacencyList.resize(vertices);

	graph._weighted = weighted ? true : false;

	if (!graph.IsWeighted())
		for (uint32_t i = 0; i < graph.GetEdges(); ++i)
		{
			uint32_t x, y;

			is >> x >> y;
			graph._adjacencyList[x].push_back(std::make_pair(y, 0));
		}
	else
		for (uint32_t i = 0; i < graph.GetEdges(); ++i)
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
	uint32_t vertices;
	ifs >> vertices >> graph._edges >> weighted;

	graph._adjacencyList.~vector();
	graph._adjacencyList.resize(vertices);

	graph._weighted = weighted ? true : false;

	if (!graph.IsWeighted())
		for (uint32_t i = 0; i < graph.GetEdges(); ++i)
		{
			uint32_t x, y;

			ifs >> x >> y;
			graph._adjacencyList[x].push_back(std::make_pair(y, 0));
		}
	else
		for (uint32_t i = 0; i < graph.GetEdges(); ++i)
		{
			uint32_t x, y, w;

			ifs >> x >> y >> w;
			graph._adjacencyList[x].push_back(std::make_pair(y, w));
		}

	return ifs;
}

