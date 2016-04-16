#pragma once

#include "PCH.h"
#include "Graph.h"

class UndirectedGraph : public Graph
{
	public:
		UndirectedGraph() : Graph() { }
		UndirectedGraph(std::ifstream& ifs);
		UndirectedGraph(const UndirectedGraph& source) : Graph(source) { }

		uint32_t getDegree(uint32_t vertex) const override;
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
		
		virtual Vector<Vector<bool>> getRoadMatrix() const override;

		virtual Vector<Vector<uint32_t>> getConnectedComponents() const;
		Vector<Vector<uint32_t>> getBiconnectedComponents() const;	// The way it gives the biconnectedComponents have to be reworked.
		Vector<uint32_t> getArticulationPoints() const;

		UndirectedGraph& operator=(const UndirectedGraph& source);

		UndirectedGraph operator+(const UndirectedGraph& source);
		UndirectedGraph operator-(const UndirectedGraph& source);

		bool operator==(const UndirectedGraph& source);
		bool operator!=(const UndirectedGraph& source) { return !((*this) == source); }

		friend std::istream& operator>>(std::istream& is, UndirectedGraph& graph);
		friend std::ifstream& operator>>(std::ifstream& ifs, UndirectedGraph& graph);

	protected:
		UndirectedGraph(uint32_t vertices) : Graph(vertices) { }

	private:
		void articulationPoint(uint32_t vertex, Vector<bool>& visited, Vector<int>& parent,
			Vector<uint32_t>& discoveryTime, Vector<uint32_t>& low, Vector<uint32_t>& articulationPoints) const;

		bool hasArticulationPoint(uint32_t vertex, Vector<bool>& visited, Vector<int>& parent,
			Vector<uint32_t>& discoveryTime, Vector<uint32_t>& low) const;

		void getBiconnectedComponents(uint32_t vertex, Vector<int>& parent, Vector<uint32_t>& depth, 
			Vector<uint32_t>& low, Stack<uint32_t>& stack, Vector<Vector<uint32_t>>& biconnectedComponents) const;
};

