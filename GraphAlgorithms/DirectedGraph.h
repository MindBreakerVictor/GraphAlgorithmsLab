#pragma once

#include "PCH.h"
#include "Graph.h"

class DirectedGraph : public Graph
{
	public:
		DirectedGraph() : Graph() { }
		DirectedGraph(std::ifstream& input);
		DirectedGraph(const DirectedGraph& source) : Graph(source._vertices, source._edges, source._adjacencyList) { }
		~DirectedGraph() { }

		uint32_t getDegree(uint32_t vertex) const override;
		uint32_t getInDegree(uint32_t vertex) const;
		uint32_t getOutDegree(uint32_t vertex) const;

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
		//Vector<Vector<uint32_t>> getStronglyConnectedComponents() const;

		DirectedGraph& operator=(const DirectedGraph& source);

		DirectedGraph operator+(const DirectedGraph& source) const;
		DirectedGraph operator-(const DirectedGraph& source) const;

		bool operator==(const DirectedGraph& source) const;
		bool operator!=(const DirectedGraph& source) const { return !((*this) == source); }

		friend std::istream& operator>>(std::istream& is, DirectedGraph& graph);
		friend std::ifstream& operator>>(std::ifstream& ifs, DirectedGraph& graph);

	private:
		void topologicalSort(uint32_t vertex, Vector<bool>& visited, Stack<uint32_t>& topSort) const;
};

