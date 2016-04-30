#ifndef _UNDIRECTED_GRAPH_H
#define _UNDIRECTED_GRAPH_H

#include "PCH.h"
#include "Graph.h"

class UndirectedGraph : public Graph
{
	public:
		UndirectedGraph() : Graph() { }
		explicit UndirectedGraph(std::ifstream& ifs, bool weighted = false);
		UndirectedGraph(UndirectedGraph const& source) : Graph(source) { }

		uint32_t GetDegree(uint32_t const& vertex) const override;

		double GetDensity() const override;

		virtual bool IsComplete() const override;
		virtual bool IsRegular() const override;
		virtual bool IsConnected() const;
		virtual bool IsHamiltonian() const;
		virtual bool IsEulerian() const;
		virtual bool IsBipartite() const;
		bool IsBiconnected() const;
		
		virtual Matrix<bool> GetRoadMatrix() const override;

		Vector<uint32_t> GetArticulationPoints() const;
		Matrix<uint32_t> GetConnectedComponents() const;
		Matrix<uint32_t> GetBiconnectedComponents() const;	// The way it gives the biconnectedComponents have to be reworked.
		Vector<Pair<uint32_t, uint32_t>> GetMinimumSpanningTree() const;
		Vector<Pair<uint32_t, uint32_t>> GetMinimumSpanningTree(int32_t* cost) const;
		Vector<Pair<Pair<uint32_t, uint32_t>, int32_t>> GetEdgesVector() const;

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
		bool HasArticulationPoint(uint32_t const& vertex, Vector<bool>* visited, Vector<int>* parent,
			Vector<uint32_t>* discoveryTime, Vector<uint32_t>* low) const;

		void ArticulationPoint(uint32_t const& vertex, Vector<bool>* visited, Vector<int>* parent,
			Vector<uint32_t>* discoveryTime, Vector<uint32_t>* low, Vector<uint32_t>* articulationPoints) const;

		void GetBiconnectedComponents(uint32_t const& vertex, Vector<int>* parent, Vector<uint32_t>* depth,
			Vector<uint32_t>* low, Stack<uint32_t>* stack, Vector<Vector<uint32_t>>* biconnectedComponents) const;

		// Hidden interface
		uint32_t GetInDegree(uint32_t const& vertex) const override { return 0; }	// Override it in case of using Graph& to an UndirectedGraph object.
		Graph::GetOutDegree;	// Equivalent to GetDegree in UndirectedGraph

		Graph::GetMinInDegree;
		Graph::GetMinOutDegree;
		Graph::GetMaxInDegree;
		Graph::GetMaxOutDegree;
};

#endif

