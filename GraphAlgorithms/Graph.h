#pragma once

#include "PCH.h"

class Graph
{
	public:
		virtual ~Graph() { }

		uint32_t getVertices() const { return _vertices; }
		uint32_t getEdges() const { return _edges; }

		virtual uint32_t getDegree(uint32_t vertex) const = 0;
		virtual uint32_t getMinDegree() const = 0;
		virtual uint32_t getMaxDegree() const = 0;

		virtual double getDensity() const { return ((double)_edges / (double)(_vertices * (_vertices - 1))); }

		virtual bool isComplete() const = 0;
		virtual bool isRegular() const = 0;

		virtual Vector<uint32_t> breadthFirstSearch(uint32_t vertex) const;
		virtual Vector<uint32_t> depthFirstSearch(uint32_t vertex) const;

		virtual Vector<Vector<bool>> getRoadMatrix() const = 0;
		virtual Vector<int> getRoadDistance(uint32_t vertex) const;

		friend std::ostream& operator<<(std::ostream& os, const Graph& graph);
		friend std::ofstream& operator<<(std::ofstream& ofs, const Graph& graph);

	protected:
		Vector<Vector<uint32_t>> _adjacencyList;
		uint32_t _vertices, _edges;

		Graph() : _vertices(0), _edges(0), _adjacencyList(0) { }
		explicit Graph(uint32_t vertices) : _vertices(vertices), _edges(vertices - 1), _adjacencyList(vertices) { }
		Graph(uint32_t vertices, uint32_t edges, Vector<Vector<uint32_t>> adjacencyList) :
			_vertices(_vertices), _edges(_edges), _adjacencyList(_adjacencyList) { }

		bool isValidVertex(uint32_t vertex) const;
};

