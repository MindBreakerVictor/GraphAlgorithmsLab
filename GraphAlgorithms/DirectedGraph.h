#ifndef _DIRECTED_GRAPH_H
#define _DIRECTED_GRAPH_H

#include "PCH.h"
#include "Graph.h"

class DirectedGraph : public Graph
{
	public:
		DirectedGraph() : Graph() { }
		explicit DirectedGraph(std::ifstream& ifs, bool weighted = false);
		DirectedGraph(DirectedGraph const& source) : Graph(source) { }

		bool IsComplete() const override;
		bool IsRegular() const override;
		bool IsStronglyConnected() const;

		Vector<Vector<bool>> GetRoadMatrix() const override;

		std::stack<uint32_t> GetTopologicalSort() const;
		Matrix<uint32_t> GetStronglyConnectedComponents() const;

		DirectedGraph& operator=(DirectedGraph const& source);

		DirectedGraph operator+(DirectedGraph const& source) const;
		DirectedGraph operator-(DirectedGraph const& source) const;

		bool operator==(DirectedGraph const& source) const;
		bool operator!=(DirectedGraph const& source) const { return !((*this) == source); }

		friend std::istream& operator>>(std::istream& is, DirectedGraph& graph);
		friend std::ifstream& operator>>(std::ifstream& ifs, DirectedGraph& graph);

	private:
		void TopologicalSort(uint32_t const& vertex, Vector<bool>* visited, Stack<uint32_t>* topSort) const;

		void GetStronglyConnectedComponents(uint32_t const& vertex, Vector<uint32_t>* depth, Vector<uint32_t>* low,
			Vector<bool>* isInStack, Stack<uint32_t>* stack, Matrix<uint32_t>* stronglyConnectedComponents) const;
};

#endif

