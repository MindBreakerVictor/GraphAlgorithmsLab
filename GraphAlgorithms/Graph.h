#ifndef _GRAPH_H
#define _GRAPH_H

#include "PCH.h"

using AdjacencyListConstIterator = Vector<Pair<uint32_t, int32_t>>::const_iterator;

class Graph
{
	public:
		virtual ~Graph() { }

		bool IsWeighted() const { return _weighted; }
		bool HasVertices() const { return (GetVertices() != 0) ? true : false; }
		bool HasEdges() const { return (GetEdges() != 0) ? true : false; }

		uint32_t GetVertices() const { return _adjacencyList.size(); }
		uint32_t GetEdges() const { return _edges; }

		virtual uint32_t GetDegree(uint32_t const& vertex) const;
		virtual uint32_t GetInDegree(uint32_t const& vertex) const;
		virtual uint32_t GetOutDegree(uint32_t const& vertex) const final;

		virtual uint32_t GetMinDegree() const;
		virtual uint32_t GetMaxDegree() const;

		virtual uint32_t GetMinInDegree() const final;
		virtual uint32_t GetMinOutDegree() const final;
		virtual uint32_t GetMaxInDegree() const final;
		virtual uint32_t GetMaxOutDegree() const final;

		virtual double GetDensity() const;

		virtual bool IsComplete() const = 0;
		virtual bool IsRegular() const = 0;

		virtual Vector<uint32_t> BreadthFirstSearch(uint32_t const& vertex) const;
		virtual Vector<uint32_t> DepthFirstSearch(uint32_t const& vertex) const;

		virtual Matrix<bool> GetRoadMatrix() const = 0;
		virtual Vector<int> GetRoadDistance(uint32_t const& vertex) const;

		friend std::ostream& operator<<(std::ostream& os, Graph const& graph);
		friend std::ofstream& operator<<(std::ofstream& ofs, Graph const& graph);

	protected:
		Graph() : _weighted(false), _edges(0), _adjacencyList(0) { }

		explicit Graph(uint32_t const& vertices) : _weighted(false), _edges(vertices - 1), 
			_adjacencyList(vertices) { }

		Graph(Graph const& source) : _weighted(source._weighted), _edges(source._edges), 
			_adjacencyList(source._adjacencyList) { }

		bool IsValidVertex(uint32_t const& vertex) const { return !(vertex > (GetVertices() - 1)); };

		bool _weighted;
		uint32_t  _edges;
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

			return firstPair.second > secondPair.second ? true : false;
		}
};

#endif

