#ifndef GRAPH_H
#define GRAPH_H

#include "PCH.h"

class Graph
{
	public:
		virtual ~Graph() { }

		uint getVertices() const { return vertices; }
		uint getEdges() const { return edges; }

		virtual uint getDegree(uint vertex) const = 0;
		virtual uint getMinDegree() const = 0;
		virtual uint getMaxDegree() const = 0;

		virtual double getDensity() const = 0;

		virtual bool isComplete() const = 0;
		virtual bool isRegular() const = 0;

		virtual std::vector<uint> breadthFirstSearch(uint vertex) const = 0;
		virtual std::vector<uint> depthFirstSearch(uint vertex) const = 0;

		virtual std::vector<std::vector<bool>> getRoadMatrix() const = 0;
		virtual std::vector<int> getRoadDistance(uint vertex) const = 0;

		friend std::ostream& operator<<(std::ostream& os, const Graph& graph);
		friend std::ofstream& operator<<(std::ofstream& ofs, const Graph& graph);

	protected:
		std::vector<std::vector<uint>> adjacencyList;
		uint vertices, edges;

		Graph() : vertices(0), edges(0), adjacencyList(0) { }
		Graph(uint _vertices, uint _edges, std::vector<std::vector<uint>> _adjacencyList) : 
			vertices(_vertices), edges(_edges), adjacencyList(_adjacencyList) { }

		bool isValidVertex(uint vertex) const;
};

#endif
