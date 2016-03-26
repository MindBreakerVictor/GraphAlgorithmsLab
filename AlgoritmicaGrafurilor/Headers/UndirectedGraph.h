#ifndef UNDIRECTED_GRAPH
#define UNDIRECTED_GRAPH

#include "Graph.h"

class UndirectedGraph : public Graph
{
	public:
		UndirectedGraph() : Graph() { }
		UndirectedGraph(std::ifstream& ifs);
		UndirectedGraph(const UndirectedGraph& source) : Graph(source.vertices, source.edges, source.adjacencyList) { }
		~UndirectedGraph() { };

		uint getDegree(uint vertex) const;
		uint getMinDegree() const;
		uint getMaxDegree() const;

		double getDensity() const { return ((2 * (double)edges) / (double)(vertices * (vertices - 1))); }

		bool isComplete() const;
		bool isRegular() const;
		bool isConnected() const;
		bool isHamiltonian() const;
		bool isEulerian() const;
		bool isBipartite() const;
		bool isBiconnected() const;

		std::vector<uint> breadthFirstSearch(uint vertex) const;
		std::vector<uint> depthFirstSearch(uint vertex) const;
		
		std::vector<std::vector<bool>> getRoadMatrix() const;
		std::vector<int> getRoadDistance(uint vertex) const;

		std::vector<std::vector<uint>> getConnectedComponents() const;
		std::vector<std::vector<uint>> getBiconnectedComponents() const;	// The way it gives the biconnectedComponents have to be reworked.
		std::vector<uint> getArticulationPoints() const;

		UndirectedGraph& operator=(const UndirectedGraph& source);

		UndirectedGraph operator+(const UndirectedGraph& source);
		UndirectedGraph operator-(const UndirectedGraph& source);

		bool operator==(const UndirectedGraph& source);
		bool operator!=(const UndirectedGraph& source) { return !((*this) == source); }

		friend std::istream& operator>>(std::istream& is, UndirectedGraph& graph);
		friend std::ifstream& operator>>(std::ifstream& ifs, UndirectedGraph& graph);

	private:
		void articulationPoint(uint vertex, std::vector<bool>& visited, std::vector<int>& parent,
			std::vector<uint>& discoveryTime, std::vector<uint>& low, std::vector<uint>& articulationPoints) const;

		bool hasArticulationPoint(uint vertex, std::vector<bool>& visited, std::vector<int>& parent,
			std::vector<uint>& discoveryTime, std::vector<uint>& low) const;

		void getBiconnectedComponents(uint vertex, std::vector<int>& parent, std::vector<uint>& depth, 
			std::vector<uint>& low, std::stack<uint>& stack, std::vector<std::vector<uint>>& biconnectedComponents) const;
};

#endif
