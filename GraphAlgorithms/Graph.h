#pragma once

#include "PCH.h"

class Graph
{
	protected:
		Graph() : _weighted(false), _vertices(0), _edges(0), _adjacencyList(0) { }
		explicit Graph(uint32_t const& vertices) : _weighted(false), _vertices(vertices), 
			_edges(vertices - 1), _adjacencyList(vertices) { }
		Graph(Graph const& source) : _weighted(source._weighted), _vertices(source._vertices), 
			_edges(source._edges), _adjacencyList(source._adjacencyList) { }

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

		virtual Vector<Vector<bool>> getRoadMatrix() const = 0;
		virtual Vector<int> getRoadDistance(uint32_t const& vertex) const;

		friend std::ostream& operator<<(std::ostream& os, Graph const& graph);
		friend std::ofstream& operator<<(std::ofstream& ofs, Graph const& graph);

	protected:
		bool isValidVertex(uint32_t const& vertex) const;

	protected:
		bool _weighted;
		uint32_t _vertices, _edges;
		Vector<Vector<Pair<uint32_t, uint32_t>>> _adjacencyList;
};

