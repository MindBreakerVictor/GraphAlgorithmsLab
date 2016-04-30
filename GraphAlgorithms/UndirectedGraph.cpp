#include "PCH.h"
#include "UndirectedGraph.h"
#include "DisjointSet.h"

UndirectedGraph::UndirectedGraph(std::ifstream& ifs, bool weighted)
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
			_adjacencyList[y].push_back(std::make_pair(x, 0));
		}
	else
		for (uint32_t i = 0; i < GetEdges(); ++i)
		{
			uint32_t x, y, w;

			ifs >> x >> y >> w;
			_adjacencyList[x].push_back(std::make_pair(y, w));
			_adjacencyList[y].push_back(std::make_pair(x, w));
		}
}

uint32_t UndirectedGraph::GetDegree(uint32_t const& vertex) const
{
	return Graph::GetOutDegree(vertex);
}

double UndirectedGraph::GetDensity() const
{
	return 2 * Graph::GetDensity();
}

bool UndirectedGraph::IsComplete() const
{
	if (!HasVertices() || !HasEdges())
		return false;

	if ((GetVertices() * (GetVertices() - 1)) / 2 != GetEdges())
		return false;

	return true;
}

bool UndirectedGraph::IsRegular() const
{
	for (uint32_t i = 1; i < GetVertices(); ++i)
		if (GetDegree(i) != GetDegree(i - 1))
			return false;

	return true;
}

bool UndirectedGraph::IsConnected() const
{
	if (!HasVertices() || !HasEdges())
		return false;

	if (GetVertices() == 1)
		return true;

	if (DepthFirstSearch(0).size() == GetVertices())
		return true;

	return false;
}

bool UndirectedGraph::IsHamiltonian() const
{
	if (GetVertices() < 3)
		return false;

	if (IsComplete())
		return true;

	for (uint32_t i = 0; i < GetVertices(); ++i)
		if (GetDegree(i) < (GetVertices() / 2))
			return false;

	return true;
}

bool UndirectedGraph::IsEulerian() const
{
	if (!IsConnected())
		return false;

	for (uint32_t i = 0; i < GetVertices(); ++i)
		if (GetDegree(i) % 2 != 0)
			return false;

	return true;
}

bool UndirectedGraph::IsBipartite() const
{
	if (!HasVertices() || !HasEdges())
		return false;

	Vector<bool> visited(GetVertices());
	Vector<char> color(GetVertices());
	Queue<uint32_t> queue;

	queue.push(0);
	visited[0] = true;
	color[0] = 0;

	while (!queue.empty())
	{
		uint32_t element = queue.front();

		for (uint32_t i = 0; i < GetDegree(element); ++i)
		{
			if (!visited[_adjacencyList[element][i].first])
			{
				queue.push(_adjacencyList[element][i].first);
				visited[_adjacencyList[element][i].first] = true;
				color[_adjacencyList[element][i].first] = (color[element] == 0 ? 1 : 0);
				continue;
			}
			
			if (color[element] == color[_adjacencyList[element][i].first])
				return false;
		}

		queue.pop();
	}

	return true;
}

bool UndirectedGraph::IsBiconnected() const
{
	if (!HasVertices() || !HasEdges())
		return false;

	Vector<bool> visited(GetVertices(), false);
	Vector<int> parent(GetVertices(), -1);
	Vector<uint32_t> discoveryTime(GetVertices());
	Vector<uint32_t> low(GetVertices());

	if (HasArticulationPoint(0, &visited, &parent, &discoveryTime, &low))
		return false;

	for (uint32_t i = 0; i < visited.size(); ++i)
		if (!visited[i])
			return false;

	return true;
}

Matrix<bool> UndirectedGraph::GetRoadMatrix() const
{
	Matrix<bool> roadMatrix(GetVertices());
	Matrix<uint32_t> connectedComponents = GetConnectedComponents();

	for (uint32_t i = 0; i < roadMatrix.size(); ++i)
		roadMatrix[i].resize(GetVertices(), false);

	for (uint32_t i = 0; i < connectedComponents.size(); ++i)
		for (uint32_t j = 0; j < connectedComponents[i].size() - 1; ++j)
			for (uint32_t k = j + 1; k < connectedComponents[i].size(); ++k)
			{
				roadMatrix[connectedComponents[i][j]][connectedComponents[i][k]] = true;
				roadMatrix[connectedComponents[i][k]][connectedComponents[i][j]] = true;
			}

	return roadMatrix;
}

Vector<uint32_t> UndirectedGraph::GetArticulationPoints() const
{
	Vector<bool> visited(GetVertices(), false);
	Vector<int> parent(GetVertices(), -1);
	Vector<uint32_t> discoveryTime(GetVertices());
	Vector<uint32_t> low(GetVertices());
	Vector<uint32_t> articulationPoints;

	for (uint32_t i = 0; i < GetVertices(); ++i)
		if (!visited[i])
			ArticulationPoint(i, &visited, &parent, &discoveryTime, &low, &articulationPoints);

	return articulationPoints;
}

Matrix<uint32_t> UndirectedGraph::GetConnectedComponents() const
{
	Matrix<uint32_t> connectedComponents;
	Vector<bool> visited(GetVertices(), false);

	for (uint32_t i = 0; i < GetVertices(); ++i)
	{
		if (!visited[i])
		{
			connectedComponents.push_back(DepthFirstSearch(i));

			for (uint32_t j = 0; j < connectedComponents.back().size(); ++j)
				visited[connectedComponents.back()[j]] = true;
		}
	}

	return connectedComponents;
}

Matrix<uint32_t> UndirectedGraph::GetBiconnectedComponents() const
{
	Stack<uint32_t> stack;
	Vector<int> parent(GetVertices(), -1);						// Represents the parent of the vertex in DFS-tree.
	Vector<uint32_t> low(GetVertices(), 0);						// Represents the lowest depth of a vertex connected to the index vertex through a back-edge in DFS-tree.
	Vector<uint32_t> depth(GetVertices(), 0);					// Represents the depth of the vertex in DFS-tree.
	Matrix<uint32_t> biconnectedComponents;

	for (uint32_t i = 0; i < GetVertices(); ++i)
		if (!depth[i])
			GetBiconnectedComponents(i, &parent, &depth, &low, &stack, &biconnectedComponents);

	return biconnectedComponents;
}

Vector<Pair<uint32_t, uint32_t>> UndirectedGraph::GetMinimumSpanningTree() const
{
	Vector<Pair<Pair<uint32_t, uint32_t>, int32_t>> edgesCostVector = GetEdgesVector();

	std::sort(edgesCostVector.begin(), edgesCostVector.end(), EdgesCostComparator(true));

	DisjointSet disjointSet(GetVertices());
	Vector<Pair<uint32_t, uint32_t>> mstEdges;	// Minimum Spanning Tree edges

	for (uint32_t i = 0; i < edgesCostVector.size(); ++i)
	{
		uint32_t x = edgesCostVector[i].first.first;
		uint32_t y = edgesCostVector[i].first.second;

		if (disjointSet.GetRoot(x) != disjointSet.GetRoot(y))
		{
			mstEdges.push_back(std::make_pair(x, y));
			disjointSet.UnionSets(x, y);
		}
	}

	return mstEdges;
}

Vector<Pair<uint32_t, uint32_t>> UndirectedGraph::GetMinimumSpanningTree(int32_t* cost) const
{
	Vector<Pair<Pair<uint32_t, uint32_t>, int32_t>> edgesCostVector = GetEdgesVector();

	std::sort(edgesCostVector.begin(), edgesCostVector.end(), EdgesCostComparator(true));

	DisjointSet disjointSet(GetVertices());
	Vector<Pair<uint32_t, uint32_t>> mstEdges;	// Minimum Spanning Tree edges

	for (uint32_t i = 0; i < edgesCostVector.size(); ++i)
	{
		int32_t _cost = edgesCostVector[i].second;
		uint32_t x = edgesCostVector[i].first.first;
		uint32_t y = edgesCostVector[i].first.second;

		if (disjointSet.GetRoot(x) != disjointSet.GetRoot(y))
		{
			mstEdges.push_back(std::make_pair(x, y));
			disjointSet.UnionSets(x, y);
			*cost += _cost;
		}
	}

	return mstEdges;
}

Vector<Pair<Pair<uint32_t, uint32_t>, int32_t>> UndirectedGraph::GetEdgesVector() const
{
	typedef Vector<Pair<Pair<uint32_t, uint32_t>, int32_t>>::const_iterator EdgesIterator;
	Vector<Pair<Pair<uint32_t, uint32_t>, int32_t>> edges;

	for (uint32_t i = 0; i < GetVertices(); ++i)
		for (AdjacencyListConstIterator itr = _adjacencyList[i].begin(); itr != _adjacencyList[i].end(); ++itr)
		{
			bool found = false;

			for (EdgesIterator iter = edges.begin(); iter != edges.end() && !found; ++iter)
				if (((iter->first.first == i) && (iter->first.second == itr->first)) || 
					((iter->first.second == i) && (iter->first.first == itr->first)))
					found = true;

			if (!found)
				edges.push_back(std::make_pair(std::make_pair(i, itr->first), itr->second));
		}

	return edges;
}

void UndirectedGraph::ArticulationPoint(uint32_t const& vertex, Vector<bool>* visited, Vector<int>* parent,
	Vector<uint32_t>* discoveryTime, Vector<uint32_t>* low, Vector<uint32_t>* articulationPoints) const
{
	if (!IsValidVertex(vertex))
		return;

	static uint32_t time = 0;
	uint32_t children = 0;
	(*visited)[vertex] = true;
	(*discoveryTime)[vertex] = (*low)[vertex] = ++time;

	for (uint32_t i = 0; i < GetDegree(vertex); ++i)
	{
		if (!(*visited)[_adjacencyList[vertex][i].first])
		{
			children++;
			(*parent)[_adjacencyList[vertex][i].first] = vertex;
			ArticulationPoint(_adjacencyList[vertex][i].first, visited, parent, discoveryTime, low, articulationPoints);

			if ((*low)[_adjacencyList[vertex][i].first] < (*low)[vertex])
				(*low)[vertex] = (*low)[_adjacencyList[vertex][i].first];

			if (((*parent)[vertex] == -1 && children > 1) ||
				((*parent)[vertex] != -1 && (*low)[_adjacencyList[vertex][i].first] >= (*discoveryTime)[vertex]))
				articulationPoints->push_back(vertex);
		}
		else if ((_adjacencyList[vertex][i].first != (*parent)[vertex]) && 
			((*discoveryTime)[_adjacencyList[vertex][i].first] < (*low)[vertex]))
			(*low)[vertex] = (*discoveryTime)[_adjacencyList[vertex][i].first];
	}
}

bool UndirectedGraph::HasArticulationPoint(uint32_t const& vertex, Vector<bool>* visited, 
	Vector<int>* parent, Vector<uint32_t>* discoveryTime, Vector<uint32_t>* low) const
{
	if (!IsValidVertex(vertex))
		return false;

	static uint32_t time = 0;
	uint32_t children = 0;
	(*visited)[vertex] = true;
	(*discoveryTime)[vertex] = (*low)[vertex] = ++time;

	for (uint32_t i = 0; i < GetDegree(vertex); ++i)
	{
		if (!(*visited)[_adjacencyList[vertex][i].first])
		{
			children++;
			(*parent)[_adjacencyList[vertex][i].first] = vertex;
			
			if (HasArticulationPoint(_adjacencyList[vertex][i].first, visited, parent, discoveryTime, low))
				return true;

			if ((*low)[_adjacencyList[vertex][i].first] < (*low)[vertex])
				(*low)[vertex] = (*low)[_adjacencyList[vertex][i].first];

			if (((*parent)[vertex] == -1 && children > 1) ||
				((*parent)[vertex] != -1 && (*low)[_adjacencyList[vertex][i].first] >= (*discoveryTime)[vertex]))
				return true;
		}
		else if ((_adjacencyList[vertex][i].first != (*parent)[vertex]) &&
			((*discoveryTime)[_adjacencyList[vertex][i].first] < (*low)[vertex]))
			(*low)[vertex] = (*discoveryTime)[_adjacencyList[vertex][i].first];
	}

	return false;
}

void UndirectedGraph::GetBiconnectedComponents(uint32_t const& vertex, Vector<int>* parent, Vector<uint32_t>* depth,
	Vector<uint32_t>* low, Stack<uint32_t>* stack, Vector<Vector<uint32_t>>* biconnectedComponents) const
{
	if (!IsValidVertex(vertex))
		return;

	static uint32_t currentDepth = 0;
	stack->push(vertex);
	(*depth)[vertex] = (*low)[vertex] = ++currentDepth;

	for (AdjacencyListConstIterator neighbour = _adjacencyList[vertex].begin(); neighbour != _adjacencyList[vertex].end(); ++neighbour)
	{
		if (!(*depth)[neighbour->first])
		{
			(*parent)[neighbour->first] = vertex;
			GetBiconnectedComponents(neighbour->first, parent, depth, low, stack, biconnectedComponents);
			(*low)[vertex] = std::min((*low)[vertex], (*low)[neighbour->first]);

			// Check if vertex is an articulation point.
			if ((*low)[neighbour->first] >= (*depth)[vertex])
			{
				biconnectedComponents->push_back(Vector<uint32_t>());
				Matrix<uint32_t>::reverse_iterator itr = biconnectedComponents->rbegin();
				while (stack->top() != neighbour->first)
				{
					itr->push_back(stack->top());
					stack->pop();
				}
				itr->push_back(neighbour->first);
				stack->pop();
				itr->push_back(vertex);
			}
		}
		// Check if neighbour is an ancestor of vertex in dfs tree.
		else if (neighbour->first != (*parent)[vertex])
			(*low)[vertex] = std::min((*low)[vertex], (*depth)[neighbour->first]);
	}
}

UndirectedGraph& UndirectedGraph::operator=(UndirectedGraph const& source)
{
	if (this == &source)
		return *this;

	_weighted = source._weighted;
	_edges = source._edges;
	_adjacencyList = source._adjacencyList;

	return *this;
}

UndirectedGraph UndirectedGraph::operator+(UndirectedGraph const& source)
{
	// TODO: better way to handle this
	if (this->GetVertices() != source.GetVertices() || this->GetVertices() == 0)
		return UndirectedGraph();

	UndirectedGraph sumGraph;
	sumGraph._adjacencyList.resize(this->GetVertices());

	for (uint32_t i = 0; i < sumGraph.GetVertices(); ++i)
	{
		for (uint32_t j = 0; j < this->GetDegree(i); ++j)
			sumGraph._adjacencyList[i].push_back(this->_adjacencyList[i][j]);

		for (uint32_t j = 0; j < source.GetDegree(i); ++j)
		{
			bool found = false;

			for (uint32_t k = 0; k < sumGraph.GetDegree(i); ++k)
				if (sumGraph._adjacencyList[i][k] == source._adjacencyList[i][j])
					found = true;

			if (!found)
				sumGraph._adjacencyList[i].push_back(source._adjacencyList[i][j]);
		}
	}

	return sumGraph;
}

UndirectedGraph UndirectedGraph::operator-(UndirectedGraph const& source)
{
	// TODO: better way to handle this
	if (this->GetVertices() != source.GetVertices() || this->GetVertices() == 0)
		return UndirectedGraph();

	UndirectedGraph difGraph;
	difGraph._adjacencyList.resize(this->GetVertices());

	for (uint32_t i = 0; i < this->GetVertices(); ++i)
	{
		for (uint32_t j = 0; j < this->GetDegree(i); ++j)
		{
			bool found = false;
			for (uint32_t k = 0; k < source.GetDegree(i); ++k)
				if (this->_adjacencyList[i][j] == source._adjacencyList[i][k])
					found = true;

			if (!found)
				difGraph._adjacencyList[i].push_back(this->_adjacencyList[i][j]);
		}
	}

	return difGraph;
}

bool UndirectedGraph::operator==(UndirectedGraph const& source)
{
	if (this->GetVertices() != source.GetVertices() || 
		this->GetEdges() != source.GetEdges() || 
		this->_adjacencyList != source._adjacencyList)
		return false;

	if (((*this) + source).GetEdges() != this->GetEdges())
		return false;

	return true;
}

std::istream& operator>>(std::istream& is, UndirectedGraph& graph)
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
			graph._adjacencyList[y].push_back(std::make_pair(x, 0));
		}
	else
		for (uint32_t i = 0; i < graph.GetEdges(); ++i)
		{
			uint32_t x, y, w;

			is >> x >> y >> w;
			graph._adjacencyList[x].push_back(std::make_pair(y, w));
			graph._adjacencyList[y].push_back(std::make_pair(x, w));
		}

	return is;
}

std::ifstream& operator>>(std::ifstream& ifs, UndirectedGraph& graph)
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
			graph._adjacencyList[y].push_back(std::make_pair(x, 0));
		}
	else
		for (uint32_t i = 0; i < graph.GetEdges(); ++i)
		{
			uint32_t x, y, w;

			ifs >> x >> y >> w;
			graph._adjacencyList[x].push_back(std::make_pair(y, w));
			graph._adjacencyList[y].push_back(std::make_pair(x, w));
		}

	return ifs;
}

