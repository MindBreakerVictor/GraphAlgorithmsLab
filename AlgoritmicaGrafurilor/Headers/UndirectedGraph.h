#ifndef _UNDIRECTED_GRAPH
#define _UNDIRECTED_GRAPH

#include "PCH.h"

class UndirectedGraph
{
	public:
		UndirectedGraph() : vertices(0), edges(0), graph(0) { }
		UndirectedGraph(std::ifstream &input);
		UndirectedGraph(const UndirectedGraph& source) : vertices(source.vertices), edges(source.edges), graph(source.graph) { }
		virtual ~UndirectedGraph();

		uint getDegree(uint vertex) const;
		uint getMinDegree() const;
		uint getMaxDegree() const;

		uint getNodes() const { return vertices; }
		uint getEdges() const { return edges; }

		double getDensity() const { return ((2 * (double)edges) / (double)(vertices * (vertices - 1))); }

		bool isComplete() const;
		bool isRegular() const;
		bool isConnected() const;
		bool isHamiltonian() const;
		bool isEulerian() const;
		bool isBipartite() const;

		std::vector<uint> breadthFirstSearch(uint vertex) const;
		std::vector<uint> depthFirstSearch(uint vertex) const;
		std::vector<std::vector<uint>> getConnectedComponents() const;
		std::vector<std::vector<bool>> getRoadMatrix() const;
		std::vector<uint> getRoadDistance(uint vertex) const;

		UndirectedGraph& operator=(const UndirectedGraph& source);

		friend std::ofstream& operator<<(std::ofstream& ofs, const UndirectedGraph& g);
		friend std::ostream& operator<<(std::ostream& os, const UndirectedGraph& g);
		friend std::ifstream& operator>>(std::ifstream& ifs, UndirectedGraph& g);
		friend std::istream& operator>>(std::istream& is, UndirectedGraph& g);

		friend UndirectedGraph operator+(const UndirectedGraph& graphOne, const UndirectedGraph& graphTwo);
		friend UndirectedGraph operator-(const UndirectedGraph& graphOne, const UndirectedGraph& graphTwo);

		friend bool operator==(const UndirectedGraph& graphOne, const UndirectedGraph& graphTwo);
		friend bool operator!=(const UndirectedGraph& graphOne, const UndirectedGraph& graphTwo) { return !(graphOne == graphTwo); }

	protected:
		UndirectedGraph(uint _vertices, uint _edges, std::vector<std::vector<uint>> _graph) : vertices(_vertices), 
			edges(_edges), graph(_graph) { }

		bool isValidVertex(uint vertex) const;

		std::vector<std::vector<uint>> graph;
		uint vertices, edges;
};

#endif
