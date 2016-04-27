#ifndef _UNDIRECTED_GRAPH_H
#define _UNDIRECTED_GRAPH_H

#include "PCH.h"
#include "Graph.h"

class UndirectedGraph : public Graph
{
	public:
		UndirectedGraph() : Graph() { }
		explicit UndirectedGraph(std::ifstream& ifs, bool const& weighted = false);
		UndirectedGraph(UndirectedGraph const& source) : Graph(source) { }

		uint32_t getDegree(uint32_t const& vertex) const override;
		uint32_t getMinDegree() const override;
		uint32_t getMaxDegree() const override;

		double getDensity() const override { return  2 * Graph::getDensity(); }

		virtual bool isComplete() const override;
		virtual bool isRegular() const override;
		virtual bool isConnected() const;
		virtual bool isHamiltonian() const;
		virtual bool isEulerian() const;
		virtual bool isBipartite() const;
		bool isBiconnected() const;
		
		virtual Matrix<bool> getRoadMatrix() const override;

		Vector<uint32_t> getArticulationPoints() const;
		Matrix<uint32_t> getConnectedComponents() const;
		Matrix<uint32_t> getBiconnectedComponents() const;	// The way it gives the biconnectedComponents have to be reworked.
		Vector<Pair<uint32_t, uint32_t>> getMinimumSpanningTree() const;
		Vector<Pair<uint32_t, uint32_t>> getMinimumSpanningTree(int32_t& cost) const;
		Vector<Pair<Pair<uint32_t, uint32_t>, int32_t>> getEdgesVector() const;

		UndirectedGraph& operator=(UndirectedGraph const& source);

		UndirectedGraph operator+(UndirectedGraph const& source);
		UndirectedGraph operator-(UndirectedGraph const& source);

		bool operator==(UndirectedGraph const& source);
		bool operator!=(UndirectedGraph const& source) { return !((*this) == source); }

		friend std::istream& operator>>(std::istream& is, UndirectedGraph& graph);
		friend std::ifstream& operator>>(std::ifstream& ifs, UndirectedGraph& graph);

	protected:
		explicit UndirectedGraph(uint32_t const& vertices) : Graph(vertices) { }

	private:
		bool hasArticulationPoint(uint32_t const& vertex, Vector<bool>& visited, Vector<int>& parent,
			Vector<uint32_t>& discoveryTime, Vector<uint32_t>& low) const;

		void articulationPoint(uint32_t const& vertex, Vector<bool>& visited, Vector<int>& parent,
			Vector<uint32_t>& discoveryTime, Vector<uint32_t>& low, Vector<uint32_t>& articulationPoints) const;

		void getBiconnectedComponents(uint32_t const& vertex, Vector<int>& parent, Vector<uint32_t>& depth,
			Vector<uint32_t>& low, Stack<uint32_t>& stack, Vector<Vector<uint32_t>>& biconnectedComponents) const;
};

#endif

