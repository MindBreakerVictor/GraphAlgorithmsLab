#ifndef _GRAPH_H
#define _GRAPH_H

#include "PCH.h"

typedef Vector<Pair<uint32_t, int32_t>>::iterator AdjacencyListIterator;
typedef Vector<Pair<uint32_t, int32_t>>::const_iterator AdjacencyListConstantIterator;
typedef Vector<Pair<uint32_t, int32_t>>::reverse_iterator AdjacencyListReverseIterator;
typedef Vector<Pair<uint32_t, int32_t>>::const_reverse_iterator AdjacencyListConstantReverseIterator;

class Graph
{
	public:
		virtual ~Graph() { }

		bool isWeighted() const { return _weighted; }

		uint32_t getVertices() const { return _vertices; }
		uint32_t getEdges() const { return _edges; }

		virtual uint32_t getDegree(uint32_t const& vertex) const = 0;
		virtual uint32_t getMinDegree() const = 0;
		virtual uint32_t getMaxDegree() const = 0;

		virtual double getDensity() const { return ((double)_edges / (double)(_vertices * (_vertices - 1))); }

		virtual bool isComplete() const = 0;
		virtual bool isRegular() const = 0;

		virtual Vector<uint32_t> breadthFirstSearch(uint32_t const& vertex) const;
		virtual Vector<uint32_t> depthFirstSearch(uint32_t const& vertex) const;

		virtual Matrix<bool> getRoadMatrix() const = 0;
		virtual Vector<int> getRoadDistance(uint32_t const& vertex) const;

		friend std::ostream& operator<<(std::ostream& os, Graph const& graph);
		friend std::ofstream& operator<<(std::ofstream& ofs, Graph const& graph);

	protected:
		Graph() : _weighted(false), _vertices(0), _edges(0), _adjacencyList(0) { }

		explicit Graph(uint32_t const& vertices) : _weighted(false), _vertices(vertices),
			_edges(vertices - 1), _adjacencyList(vertices) { }

		Graph(Graph const& source) : _weighted(source._weighted), _vertices(source._vertices),
			_edges(source._edges), _adjacencyList(source._adjacencyList) { }

		bool isValidVertex(uint32_t const& vertex) const;

		bool _weighted;
		uint32_t _vertices, _edges;
		Matrix<Pair<uint32_t, int32_t>> _adjacencyList;	// It's an adjacency list dispite the type name.
};

class EdgesCostComparator
{
	public:
		EdgesCostComparator(bool ascending) : _ascending(ascending) { }

		bool operator()(Pair<Pair<uint32_t, uint32_t>, int32_t> const& firstEdge,
			Pair<Pair<uint32_t, uint32_t>, int32_t> const& secondEdge) const
		{
			if (_ascending)
				return firstEdge.second < secondEdge.second ? true : false;
			else
				return firstEdge.second > secondEdge.second ? true : false;
		}

	private:
		bool _ascending;
};

template <bool ascending>
class VerticesCostComparator
{
	public:
		bool operator()(Pair<uint32_t, int32_t> const& firstPair,
			Pair<uint32_t, int32_t> const& secondPair) const
		{
			if (ascending)
				return firstPair.second < secondPair.second ? true : false;
			else
				return firstPair.second > secondPair.second ? true : false;
		}
};

#endif

