#pragma once

#include "PCH.h"
#include "UndirectedGraph.h"

class Tree : public UndirectedGraph
{
	public:
		Tree() : UndirectedGraph() { }
		Tree(uint32_t vertices);
		Tree(std::ifstream& ifs);

		uint32_t getDiameter() const;
		uint32_t getRadius() const { return (uint32_t)(getDiameter() / 2); }

		bool isComplete() const { return false; }
		bool isRegular() const { return false; }
		bool isConnected() const { return true; }
		bool isHamiltonian() const { return false; }
		bool isEulerian() const { return false; }
		bool isBipartite() const { return true; }

		Vector<Vector<bool>> getRoadMatrix() const { return Vector<Vector<bool>>(_vertices, Vector<bool>(_vertices, true)); }
		Vector<Vector<uint32_t>> getConnectedComponents() const { return _adjacencyList; }
};

