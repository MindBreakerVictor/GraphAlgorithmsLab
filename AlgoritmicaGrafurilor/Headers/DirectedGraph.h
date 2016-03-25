#ifndef DIRECTED_GRAPH
#define DIRECTED_GRAPH

#include "Graph.h"

class DirectedGraph : public Graph
{
	public:
		DirectedGraph() : Graph() { }
		DirectedGraph(std::ifstream& input);
		DirectedGraph(const DirectedGraph& source) : Graph(source.vertices, source.edges, source.adjacencyList) { }
		~DirectedGraph() { }

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

		std::vector<uint> breadthFirstSearch(uint vertex) const;
		std::vector<uint> depthFirstSearch(uint vertex) const;

		std::vector<std::vector<bool>> getRoadMatrix() const;
		std::vector<int> getRoadDistance(uint vertex) const;

		std::stack<uint> getTopologicalSort() const;
		//std::vector<std::vector<uint>> getStronglyConnectedComponents() const;

		DirectedGraph& operator=(const DirectedGraph& source);

		DirectedGraph operator+(const DirectedGraph& source) const;
		DirectedGraph operator-(const DirectedGraph& source) const;

		bool operator==(const DirectedGraph& source) const;
		bool operator!=(const DirectedGraph& source) const { return !((*this) == source); }

		friend std::istream& operator>>(std::istream& is, DirectedGraph& graph);
		friend std::ifstream& operator>>(std::ifstream& ifs, DirectedGraph& graph);

	private:
		void topologicalSort(uint vertex, std::vector<bool>& visited, std::stack<uint>& topSort) const;
};

#endif
