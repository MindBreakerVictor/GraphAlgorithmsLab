#ifndef _DIRECTED_GRAPH
#define _DIRECTED_GRAPH

#include "UndirectedGraph.h"

class DirectedGraph : public UndirectedGraph
{
	public:
		DirectedGraph() : UndirectedGraph() { }
		DirectedGraph(std::ifstream& input);
		DirectedGraph(const DirectedGraph& source) : UndirectedGraph(source.vertices, source.edges, source.graph) { }
		~DirectedGraph() { UndirectedGraph::~UndirectedGraph(); }

		uint getDegree(uint vertex) const;
		uint getInDegree(uint vertex) const;
		uint getOutDegree(uint vertex) const;

		uint getMinDegree() const;
		uint getMinInDegree() const;
		uint getMinOutDegree() const;
		uint getMaxDegree() const;
		uint getMaxInDegree() const;
		uint getMaxOutDegree() const;

		double getDensity() const { return ((double)edges / (double)(vertices * (vertices - 1))); }

		bool isComplete() const;
		bool isRegular() const;
		bool isStronglyConnected() const;

		std::stack<uint> getTopologicalSort() const;
		std::vector<uint> breadthFirstSearch(uint vertex) const;
		std::vector<uint> depthFirstSearch(uint vertex) const;
		std::vector<int> getRoadDistance(uint vertex) const;
		std::vector<std::vector<bool>> getRoadMatrix() const;

		DirectedGraph& operator=(const DirectedGraph& source);

		friend DirectedGraph operator+(const DirectedGraph& graphOne, const DirectedGraph& graphTwo);
		friend DirectedGraph operator-(const DirectedGraph& graphOne, const DirectedGraph& graphTwo);

		friend bool operator==(const DirectedGraph& graphOne, const DirectedGraph& graphTwo);
		friend bool operator!=(const DirectedGraph& graphOne, const DirectedGraph& graphTwo) { return !(graphOne == graphTwo); }

	private:
		void topologicalSort(uint vertex, std::vector<bool>& visited, std::stack<uint>& topSort) const;

		// Undefined methods for DirectedGraph				
		UndirectedGraph::isHamiltonian;
		UndirectedGraph::isEulerian;
		UndirectedGraph::isBipartite;

		// Not used in a directed graph
		UndirectedGraph::getConnectedComponents;
		UndirectedGraph::isConnected;
};

#endif
