#ifndef _DIRECTED_GRAPH_H
#define _DIRECTED_GRAPH_H

#include "PCH.h"
#include "Graph.h"

class DirectedGraph : public Graph
{
	public:
		DirectedGraph() : Graph() { }
		explicit DirectedGraph(std::ifstream& ifs, bool const& weighted = false);
		DirectedGraph(DirectedGraph const& source) : Graph(source) { }

		uint32_t getDegree(uint32_t const& vertex) const override;
		uint32_t getInDegree(uint32_t const& vertex) const;
		uint32_t getOutDegree(uint32_t const& vertex) const;

		uint32_t getMinDegree() const override;
		uint32_t getMinInDegree() const;
		uint32_t getMinOutDegree() const;
		uint32_t getMaxDegree() const override;
		uint32_t getMaxInDegree() const;
		uint32_t getMaxOutDegree() const;

		bool isComplete() const override;
		bool isRegular() const override;
		bool isStronglyConnected() const;

		Vector<Vector<bool>> getRoadMatrix() const override;

		std::stack<uint32_t> getTopologicalSort() const;
		Matrix<uint32_t> getStronglyConnectedComponents() const;

		DirectedGraph& operator=(DirectedGraph const& source);

		DirectedGraph operator+(DirectedGraph const& source) const;
		DirectedGraph operator-(DirectedGraph const& source) const;

		bool operator==(DirectedGraph const& source) const;
		bool operator!=(DirectedGraph const& source) const { return !((*this) == source); }

		friend std::istream& operator>>(std::istream& is, DirectedGraph& graph);
		friend std::ifstream& operator>>(std::ifstream& ifs, DirectedGraph& graph);

	private:
		void topologicalSort(uint32_t const& vertex, Vector<bool>& visited, Stack<uint32_t>& topSort) const;

		void getStronglyConnectedComponents(uint32_t const& vertex, Vector<uint32_t>& depth, Vector<uint32_t>& low,
			Vector<bool>& isInStack, Stack<uint32_t>& stack, Matrix<uint32_t>& stronglyConnectedComponents) const;
};

#endif

